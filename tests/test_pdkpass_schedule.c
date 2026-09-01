#include <assert.h>
#include "pdkpass_data.h"
#include "pdkpass_schedule.h"

int main(void)
{
    assert(pdkpass_race_count == 11);
    assert(pdkpass_schedule_next_race(0, pdkpass_races,
                                      pdkpass_race_count) == 0);
    assert(pdkpass_schedule_next_race(1788713999, pdkpass_races,
                                      pdkpass_race_count) == 0);
    assert(pdkpass_schedule_next_race(1788714000, pdkpass_races,
                                      pdkpass_race_count) == 1);
    assert(pdkpass_schedule_next_race(1796576399, pdkpass_races,
                                      pdkpass_race_count) == 10);
    assert(pdkpass_schedule_next_race(1796576400, pdkpass_races,
                                      pdkpass_race_count) == pdkpass_race_count);
    assert(pdkpass_schedule_next_race(0, NULL, 0) == 0);

    // The Italy switch boundary is 01:00 Beijing time. Before midnight the
    // next check is midnight; at midnight it is the race boundary one hour on.
    assert(pdkpass_schedule_next_check(1788710399, pdkpass_races,
                                       pdkpass_race_count) == 1788710400);
    assert(pdkpass_schedule_next_check(1788710400, pdkpass_races,
                                       pdkpass_race_count) == 1788714000);
    assert(pdkpass_schedule_next_check(1788714000, pdkpass_races,
                                       pdkpass_race_count) == 1788796800);
    return 0;
}
