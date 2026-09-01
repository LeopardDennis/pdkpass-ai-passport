#pragma once

#include "pdkpass_data.h"
#include <stddef.h>
#include <stdint.h>

// Returns race_count when the season has completed. Each race remains current
// until its explicit switch_at_utc boundary.
size_t pdkpass_schedule_next_race(int64_t now_utc,
                                  const pdkpass_race_t *races,
                                  size_t race_count);

// Return the next UTC epoch when the home race should be reconsidered: either
// Beijing midnight or the current round's switch boundary, whichever is first.
int64_t pdkpass_schedule_next_check(int64_t now_utc,
                                    const pdkpass_race_t *races,
                                    size_t race_count);
