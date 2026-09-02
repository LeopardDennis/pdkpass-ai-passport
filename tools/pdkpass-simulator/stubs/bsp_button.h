#pragma once

#include "esp_err.h"

typedef enum {
    BSP_BTN_UP = 0,
    BSP_BTN_DOWN,
    BSP_BTN_OK,
    BSP_BTN_COUNT,
} bsp_btn_t;

typedef enum {
    BSP_BTN_PRESS = 0,
    BSP_BTN_CLICK,
    BSP_BTN_DOUBLE,
    BSP_BTN_LONG,
} bsp_btn_ev_t;

