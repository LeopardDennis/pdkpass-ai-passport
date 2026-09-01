#include <assert.h>
#include <string.h>

#include "pdkpass_season_core.h"

int main(void)
{
    // Beijing crosses into 2027 at 2026-12-31 16:00:00 UTC.
    assert(pdkpass_beijing_year(1798732799) == 2026);
    assert(pdkpass_beijing_year(1798732800) == 2027);
    assert(pdkpass_next_beijing_midnight(1798732799) == 1798732800);
    assert(pdkpass_next_beijing_midnight(1798732800) == 1798819200);

    assert(pdkpass_season_candidate_valid(2026, 2026, 23));
    assert(pdkpass_season_candidate_valid(2026, 2027, 24));
    assert(!pdkpass_season_candidate_valid(2026, 2027, 0));
    assert(!pdkpass_season_candidate_valid(2027, 2026, 23));
    assert(!pdkpass_season_candidate_valid(2027, 2027, 25));

    char text[32];
    pdkpass_format_beijing_weekend(1788517800, 1788706800, text,
                                   sizeof(text));
    assert(strcmp(text, "04-06 SEP") == 0);
    pdkpass_format_beijing_session("FP1", 1788517800, text, sizeof(text));
    assert(strcmp(text, "FP1   04 SEP 18:30") == 0);
    pdkpass_format_beijing_date(1788517800, text, sizeof(text));
    assert(strcmp(text, "04 SEP") == 0);
    return 0;
}
