#include "pdkpass_screenshot.h"

#include "bsp_display.h"
#include "driver/usb_serial_jtag_vfs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define SCREENSHOT_COMMAND "FAP_SCREENSHOT_V1"
#define SCREENSHOT_WIDTH 240
#define SCREENSHOT_HEIGHT 320
#define SCREENSHOT_BYTES (SCREENSHOT_WIDTH * SCREENSHOT_HEIGHT * 2)
#define SCREENSHOT_TASK_STACK 4096

static const char *TAG = "pdkpass_capture";
static lv_display_t *s_display;
static bool s_capture_active;
static bool s_capture_failed;
static size_t s_capture_bytes;
static int32_t s_next_row;

static bool write_all(const uint8_t *data, size_t length)
{
    while (length > 0) {
        ssize_t written = write(STDOUT_FILENO, data, length);
        if (written > 0) {
            data += written;
            length -= (size_t)written;
            continue;
        }
        if (written < 0 && (errno == EAGAIN || errno == EINTR)) {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }
        return false;
    }
    return true;
}

// LVGL renders a full invalidated screen into the existing 20-row draw buffer.
// Observing FLUSH_START lets us stream those rows in display order without a
// second full-screen framebuffer, which would exceed this wearable's RAM budget.
static void on_flush_start(lv_event_t *event)
{
    if (!s_capture_active || s_capture_failed) return;

    lv_display_t *display = lv_event_get_target(event);
    const lv_area_t *area = lv_event_get_param(event);
    lv_draw_buf_t *draw_buf = lv_display_get_buf_active(display);
    if (!area || !draw_buf || !draw_buf->data ||
        area->x1 != 0 || area->x2 != SCREENSHOT_WIDTH - 1 ||
        area->y1 != s_next_row || area->y2 >= SCREENSHOT_HEIGHT ||
        draw_buf->header.stride < SCREENSHOT_WIDTH * 2) {
        s_capture_failed = true;
        return;
    }

    const size_t row_bytes = SCREENSHOT_WIDTH * 2;
    const int32_t row_count = area->y2 - area->y1 + 1;
    for (int32_t row = 0; row < row_count; row++) {
        const uint8_t *pixels = draw_buf->data + row * draw_buf->header.stride;
        if (!write_all(pixels, row_bytes)) {
            s_capture_failed = true;
            return;
        }
        s_capture_bytes += row_bytes;
    }
    s_next_row = area->y2 + 1;
}

static void capture_current_screen(void)
{
    if (!bsp_lvgl_lock(2000)) return;

    // Raw RGB565 payloads may contain 0x0a, so disable newline translation.
    // Suppress logs only while binary bytes follow the response header.
    usb_serial_jtag_vfs_set_tx_line_endings(ESP_LINE_ENDINGS_LF);
    esp_log_level_set("*", ESP_LOG_NONE);

    static const char header[] =
        "FAP_SCREENSHOT_V1 240 320 RGB565LE 153600\n";
    s_capture_bytes = 0;
    s_next_row = 0;
    s_capture_failed = !write_all((const uint8_t *)header, sizeof(header) - 1);
    s_capture_active = !s_capture_failed;

    if (s_capture_active) {
        lv_obj_invalidate(lv_screen_active());
        lv_refr_now(s_display);
    }
    s_capture_active = false;
    fsync(STDOUT_FILENO);

    esp_log_level_set("*", ESP_LOG_INFO);
    bsp_lvgl_unlock();

    if (s_capture_failed || s_capture_bytes != SCREENSHOT_BYTES ||
        s_next_row != SCREENSHOT_HEIGHT) {
        ESP_LOGE(TAG, "Screenshot stream incomplete: %u/%u bytes, next row %ld",
                 (unsigned)s_capture_bytes, (unsigned)SCREENSHOT_BYTES,
                 (long)s_next_row);
    }
}

static void screenshot_task(void *arg)
{
    (void)arg;
    char line[48];
    size_t used = 0;

    while (true) {
        uint8_t byte;
        ssize_t received = read(STDIN_FILENO, &byte, 1);
        if (received != 1) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        if (byte == '\r') continue;
        if (byte == '\n') {
            line[used] = '\0';
            if (strcmp(line, SCREENSHOT_COMMAND) == 0) {
                capture_current_screen();
            }
            used = 0;
        } else if (used + 1 < sizeof(line)) {
            line[used++] = (char)byte;
        } else {
            used = 0;
        }
    }
}

esp_err_t pdkpass_screenshot_start(lv_display_t *display)
{
    if (!display) return ESP_ERR_INVALID_ARG;
    if (s_display) return ESP_ERR_INVALID_STATE;

    s_display = display;
    lv_display_add_event_cb(display, on_flush_start, LV_EVENT_FLUSH_START, NULL);
    BaseType_t created = xTaskCreate(screenshot_task, "fap_capture",
                                     SCREENSHOT_TASK_STACK, NULL, 3, NULL);
    if (created != pdPASS) {
        s_display = NULL;
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}
