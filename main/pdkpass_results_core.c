#include "pdkpass_results_core.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

static bool parse_two_digits(const char *text, unsigned *value)
{
    if (!isdigit((unsigned char)text[0]) ||
        !isdigit((unsigned char)text[1])) return false;
    *value = (unsigned)(text[0] - '0') * 10U + (unsigned)(text[1] - '0');
    return true;
}

static bool is_leap_year(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static unsigned days_in_month(int year, unsigned month)
{
    static const unsigned days[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    };
    if (month == 2 && is_leap_year(year)) return 29;
    return month >= 1 && month <= 12 ? days[month - 1] : 0;
}

// Days since 1970-01-01. This civil-calendar conversion is independent of the
// process timezone, which keeps host tests and the device implementation equal.
static int64_t days_from_civil(int year, unsigned month, unsigned day)
{
    year -= month <= 2;
    int era = (year >= 0 ? year : year - 399) / 400;
    unsigned year_of_era = (unsigned)(year - era * 400);
    unsigned adjusted_month = month > 2 ? month - 3U : month + 9U;
    unsigned day_of_year = (153U * adjusted_month + 2U) / 5U + day - 1U;
    unsigned day_of_era = year_of_era * 365U + year_of_era / 4U -
                          year_of_era / 100U + day_of_year;
    return (int64_t)era * 146097LL + day_of_era - 719468LL;
}

const char *pdkpass_session_label(pdkpass_session_kind_t kind)
{
    static const char *labels[PDKPASS_SESSION_COUNT] = {
        "FP1", "FP2", "FP3", "SPRINT QUALI", "SPRINT", "QUALIFYING", "RACE",
    };
    return kind < PDKPASS_SESSION_COUNT ? labels[kind] : "SESSION";
}

pdkpass_session_kind_t pdkpass_session_kind_from_name(const char *name)
{
    if (!name) return PDKPASS_SESSION_COUNT;
    if (strcmp(name, "Practice 1") == 0) return PDKPASS_SESSION_FP1;
    if (strcmp(name, "Practice 2") == 0) return PDKPASS_SESSION_FP2;
    if (strcmp(name, "Practice 3") == 0) return PDKPASS_SESSION_FP3;
    if (strcmp(name, "Sprint Qualifying") == 0 ||
        strcmp(name, "Sprint Shootout") == 0) {
        return PDKPASS_SESSION_SPRINT_QUALIFYING;
    }
    if (strcmp(name, "Sprint") == 0 || strcmp(name, "Sprint Race") == 0) {
        return PDKPASS_SESSION_SPRINT;
    }
    if (strcmp(name, "Qualifying") == 0) return PDKPASS_SESSION_QUALIFYING;
    if (strcmp(name, "Race") == 0) return PDKPASS_SESSION_RACE;
    return PDKPASS_SESSION_COUNT;
}

bool pdkpass_parse_iso8601_utc(const char *text, int64_t *epoch_utc)
{
    if (!text || !epoch_utc || strlen(text) < 19 || text[4] != '-' ||
        text[7] != '-' || (text[10] != 'T' && text[10] != ' ') ||
        text[13] != ':' || text[16] != ':') return false;

    unsigned century;
    unsigned year_low;
    unsigned month;
    unsigned day;
    unsigned hour;
    unsigned minute;
    unsigned second;
    if (!parse_two_digits(text, &century) ||
        !parse_two_digits(text + 2, &year_low) ||
        !parse_two_digits(text + 5, &month) ||
        !parse_two_digits(text + 8, &day) ||
        !parse_two_digits(text + 11, &hour) ||
        !parse_two_digits(text + 14, &minute) ||
        !parse_two_digits(text + 17, &second)) return false;

    int year = (int)(century * 100U + year_low);
    if (year < 1970 || year > 2100 || month < 1 || month > 12 || day < 1 ||
        day > days_in_month(year, month) || hour > 23 || minute > 59 ||
        second > 60) return false;

    const char *zone = text + 19;
    if (*zone == '.') {
        zone++;
        while (isdigit((unsigned char)*zone)) zone++;
    }

    int offset_seconds = 0;
    if (*zone == 'Z' || *zone == 'z' || *zone == '\0') {
        if (*zone != '\0' && zone[1] != '\0') return false;
    } else if (*zone == '+' || *zone == '-') {
        int sign = *zone == '+' ? 1 : -1;
        unsigned offset_hour;
        unsigned offset_minute;
        zone++;
        if (!parse_two_digits(zone, &offset_hour)) return false;
        zone += 2;
        if (*zone == ':') zone++;
        if (!parse_two_digits(zone, &offset_minute) || zone[2] != '\0' ||
            offset_hour > 23 || offset_minute > 59) return false;
        offset_seconds = sign * (int)(offset_hour * 3600U +
                                      offset_minute * 60U);
    } else {
        return false;
    }

    int64_t epoch = days_from_civil(year, month, day) * 86400LL;
    epoch += (int64_t)hour * 3600LL + (int64_t)minute * 60LL + second;
    *epoch_utc = epoch - offset_seconds;
    return true;
}

bool pdkpass_session_result_due(int64_t now_utc, int64_t session_end_utc)
{
    return session_end_utc > 0 &&
           now_utc >= session_end_utc + PDKPASS_RESULT_DELAY_SECONDS;
}

bool pdkpass_result_cache_identity_matches(unsigned cached_year,
                                           size_t cached_race_count,
                                           unsigned season_year,
                                           size_t season_race_count)
{
    return cached_year == season_year && cached_race_count == season_race_count &&
           season_race_count > 0U && season_race_count <= 24U;
}
