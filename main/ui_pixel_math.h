#pragma once

#include <stdint.h>

int ui_pixel_blink_frame(uint32_t elapsed_ms);
int ui_pixel_jump_offset(unsigned frame);
int ui_pixel_fit_scale(int natural_width, int available_width, int max_scale);
