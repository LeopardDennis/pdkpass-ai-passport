// PDKPASS application entry point for FoloToy AI Passport.
#include "bsp_battery.h"
#include "bsp_button.h"
#include "bsp_display.h"
#include "bsp_pins.h"
#include "esp_log.h"
#include "pdkpass_network.h"
#include "pdkpass_screenshot.h"
#include "pdkpass_ui.h"

static const char *TAG = "pdkpass";

static void on_network(const pdkpass_network_update_t *update)
{
    if (!bsp_lvgl_lock(500)) return;
    pdkpass_ui_network_update(update);
    bsp_lvgl_unlock();
}

// Button callbacks run outside the LVGL task. Keep the callback lightweight and
// hold the BSP LVGL lock for the complete UI state transition.
static void on_key(bsp_btn_t btn, bsp_btn_ev_t ev, void *user)
{
    (void)user;
    if (!bsp_lvgl_lock(500)) return;
    pdkpass_ui_key(btn, ev);
    bsp_lvgl_unlock();
}

void app_main(void)
{
    ESP_LOGI(TAG, "PDKPASS starting");

    lv_display_t *display = NULL;
    if (bsp_display_init() != ESP_OK || !(display = bsp_lvgl_init())) {
        ESP_LOGE(TAG,
                 "Display/LVGL init failed (MOSI=%d SCLK=%d CS=%d DC=%d BL=%d)",
                 BSP_LCD_MOSI, BSP_LCD_SCLK, BSP_LCD_CS, BSP_LCD_DC, BSP_LCD_BL);
        return;
    }
    bsp_display_backlight(100);

    // Battery support is optional. The UI renders BAT -- when the fuel gauge is
    // not installed or cannot be read.
    bool battery_available = bsp_battery_init() == ESP_OK;
    if (bsp_lvgl_lock(1000)) {
        pdkpass_ui_enter(battery_available);
        bsp_lvgl_unlock();
    }

    if (bsp_button_init(on_key, NULL) != ESP_OK) {
        ESP_LOGE(TAG, "Button init failed; the current screen remains readable");
    }

    if (pdkpass_screenshot_start(display) != ESP_OK) {
        ESP_LOGE(TAG, "Release screenshot service failed to start");
    }

    if (pdkpass_network_start(on_network) != ESP_OK) {
        ESP_LOGE(TAG, "Network time service failed to start");
    }

    ESP_LOGI(TAG, "PDKPASS ready; battery=%d", battery_available);
}
