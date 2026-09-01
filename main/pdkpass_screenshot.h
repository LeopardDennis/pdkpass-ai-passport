#pragma once

#include "esp_err.h"
#include "lvgl.h"

// Starts the observational FAP_SCREENSHOT_V1 serial service used by the
// official community publisher. The service never changes device state.
esp_err_t pdkpass_screenshot_start(lv_display_t *display);
