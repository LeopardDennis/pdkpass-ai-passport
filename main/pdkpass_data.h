#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    unsigned round;
    const char *country;
    const char *circuit;
    const char *weekend;
    const char *session_one_cn;
    const char *session_two_cn;
    const char *race_cn;
    unsigned circuit_length_m;
    unsigned laps;
    uint32_t accent;
} pdkpass_race_t;

typedef struct {
    unsigned position;
    const char *code;
    const char *name;
    const char *team;
    unsigned points;
    uint32_t accent;
} pdkpass_driver_t;

extern const pdkpass_race_t pdkpass_races[];
extern const size_t pdkpass_race_count;
extern const pdkpass_driver_t pdkpass_drivers[];
extern const size_t pdkpass_driver_count;
