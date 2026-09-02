#include "ui_pixel_math.h"

int ui_pixel_blink_frame(uint32_t elapsed_ms)
{
    uint32_t phase = elapsed_ms % 2000U;
    return phase >= 1650U && phase < 1800U;
}

int ui_pixel_jump_offset(unsigned frame)
{
    static const int offsets[] = { 0, -3, -5, -3, 0 };
    return frame < sizeof(offsets) / sizeof(offsets[0]) ? offsets[frame] : 0;
}

int ui_pixel_fit_scale(int natural_width, int available_width, int max_scale)
{
    if (natural_width <= 0 || available_width <= 0 || max_scale <= 0) {
        return max_scale;
    }
    int64_t fitted = (int64_t)available_width * 256 / natural_width;
    if (fitted > max_scale) fitted = max_scale;
    if (fitted < 1) fitted = 1;
    return (int)fitted;
}
