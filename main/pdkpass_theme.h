#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t top;
    uint32_t bottom;
} pdkpass_theme_t;

// Returns the stable visual theme for a known circuit. Circuit-based lookup
// keeps colours attached to venues even when future seasons change round order.
bool pdkpass_theme_get(const char *circuit, pdkpass_theme_t *theme);
