#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    const uint8_t *xy;
    size_t point_count;
} pdkpass_track_geometry_t;

bool pdkpass_track_get(const char *circuit,
                       pdkpass_track_geometry_t *geometry);
