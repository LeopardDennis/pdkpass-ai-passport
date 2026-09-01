#include "pdkpass_schedule.h"

#define SECONDS_PER_DAY 86400LL
#define BEIJING_UTC_OFFSET 28800LL

size_t pdkpass_schedule_next_race(int64_t now_utc,
                                  const pdkpass_race_t *races,
                                  size_t race_count)
{
    if (!races) return race_count;
    for (size_t i = 0; i < race_count; i++) {
        if (now_utc < races[i].switch_at_utc) return i;
    }
    return race_count;
}

int64_t pdkpass_schedule_next_check(int64_t now_utc,
                                    const pdkpass_race_t *races,
                                    size_t race_count)
{
    int64_t beijing_day = (now_utc + BEIJING_UTC_OFFSET) / SECONDS_PER_DAY;
    int64_t next_midnight = (beijing_day + 1) * SECONDS_PER_DAY -
                            BEIJING_UTC_OFFSET;
    size_t race = pdkpass_schedule_next_race(now_utc, races, race_count);
    if (race < race_count && races[race].switch_at_utc < next_midnight) {
        return races[race].switch_at_utc;
    }
    return next_midnight;
}
