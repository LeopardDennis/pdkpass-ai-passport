#include <assert.h>
#include <string.h>

#include "pdkpass_results_core.h"

int main(void)
{
    assert(pdkpass_session_kind_from_name("Practice 1") == PDKPASS_SESSION_FP1);
    assert(pdkpass_session_kind_from_name("Practice 2") == PDKPASS_SESSION_FP2);
    assert(pdkpass_session_kind_from_name("Practice 3") == PDKPASS_SESSION_FP3);
    assert(pdkpass_session_kind_from_name("Sprint Qualifying") ==
           PDKPASS_SESSION_SPRINT_QUALIFYING);
    assert(pdkpass_session_kind_from_name("Sprint Shootout") ==
           PDKPASS_SESSION_SPRINT_QUALIFYING);
    assert(pdkpass_session_kind_from_name("Sprint Race") ==
           PDKPASS_SESSION_SPRINT);
    assert(pdkpass_session_kind_from_name("Qualifying") ==
           PDKPASS_SESSION_QUALIFYING);
    assert(pdkpass_session_kind_from_name("Race") == PDKPASS_SESSION_RACE);
    assert(pdkpass_session_kind_from_name("Pre-Season Testing") ==
           PDKPASS_SESSION_COUNT);
    assert(strcmp(pdkpass_session_label(PDKPASS_SESSION_SPRINT), "SPRINT") == 0);

    int64_t epoch = 0;
    assert(pdkpass_parse_iso8601_utc("2025-09-05T12:30:00+00:00", &epoch));
    assert(epoch == 1757075400LL);
    assert(pdkpass_parse_iso8601_utc("2025-09-05T20:30:00+08:00", &epoch));
    assert(epoch == 1757075400LL);
    assert(pdkpass_parse_iso8601_utc(
        "2025-09-05T12:30:00.123456Z", &epoch));
    assert(epoch == 1757075400LL);
    assert(!pdkpass_parse_iso8601_utc("2025-02-29T12:30:00Z", &epoch));
    assert(!pdkpass_parse_iso8601_utc("not-a-date", &epoch));

    assert(!pdkpass_session_result_due(1757077199LL, 1757075400LL));
    assert(pdkpass_session_result_due(1757077200LL, 1757075400LL));
    assert(!pdkpass_session_result_due(1757077200LL, 0));

    assert(pdkpass_result_cache_identity_matches(2026, 23, 2026, 23));
    assert(!pdkpass_result_cache_identity_matches(2026, 23, 2027, 23));
    assert(!pdkpass_result_cache_identity_matches(2027, 22, 2027, 23));
    assert(!pdkpass_result_cache_identity_matches(2027, 0, 2027, 0));
    return 0;
}
