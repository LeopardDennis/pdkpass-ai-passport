#include "pdkpass_ui.h"

#include "bsp_battery.h"
#include "bsp_display.h"
#include "pdkpass_data.h"
#include "pdkpass_model.h"
#include "pdkpass_results.h"
#include "pdkpass_schedule.h"
#include "pdkpass_season.h"
#include "ui_pixel.h"
#include "lvgl.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define CONTENT_X 10
#define CONTENT_Y 53
#define CONTENT_W 220
#define CONTENT_H 211
#define CALENDAR_ROWS 5
#define STANDINGS_ROWS 6
#define IDLE_DIM_SECONDS 30
#define IDLE_OFF_SECONDS 90
#define CLOCK_FALLBACK_PERIOD_MS 86400000U

static lv_obj_t *s_screen;
static lv_obj_t *s_content;
static lv_obj_t *s_hint;
static lv_obj_t *s_battery;
static lv_obj_t *s_network;
static lv_obj_t *s_mascot;
static lv_timer_t *s_battery_timer;
static lv_timer_t *s_idle_timer;
static lv_timer_t *s_clock_timer;
static pdkpass_state_t s_state;
static pdkpass_season_snapshot_t s_season;
static bool s_battery_available;
static bool s_time_valid;
static unsigned s_idle_seconds;
static unsigned s_idle_stage;
static pdkpass_network_state_t s_network_state = PDKPASS_NETWORK_STARTING;
static char s_setup_ssid[33];
static char s_setup_password[16];

static lv_obj_t *make_block(lv_obj_t *parent, int x, int y, int w, int h,
                            uint32_t color)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(color), 0);
    return obj;
}

static lv_obj_t *make_label(lv_obj_t *parent, const char *text, int x, int y,
                            int width, const lv_font_t *font, uint32_t color)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
    lv_obj_set_pos(label, x, y);
    lv_obj_set_width(label, width);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
    return label;
}

static uint32_t contrast_color(uint32_t color)
{
    unsigned r = (color >> 16) & 0xff;
    unsigned g = (color >> 8) & 0xff;
    unsigned b = color & 0xff;
    return r * 299 + g * 587 + b * 114 > 150000 ? UI_INK : 0xFFFFFF;
}

static void content_reset(uint32_t background)
{
    lv_obj_clean(s_content);
    lv_obj_set_style_bg_color(s_content, lv_color_hex(background), 0);
}

static void set_hint(const char *text)
{
    lv_label_set_text(s_hint, text);
}

static void render_wifi_setup(void)
{
    content_reset(UI_PAPER);
    make_label(s_content, "WIFI SETUP", 0, 0, 198,
               &lv_font_montserrat_20, UI_INK);
    make_label(s_content, "1  CONNECT PHONE TO", 0, 34, 198,
               &lv_font_montserrat_14, UI_SKY_DARK);
    make_label(s_content, s_setup_ssid, 0, 58, 198,
               &lv_font_montserrat_14, UI_RED);
    make_label(s_content, "PASSWORD", 0, 88, 80,
               &lv_font_montserrat_14, UI_SKY_DARK);
    make_label(s_content, s_setup_password, 0, 111, 198,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, "2  OPEN IN BROWSER", 0, 145, 198,
               &lv_font_montserrat_14, UI_SKY_DARK);
    make_label(s_content, "192.168.4.1", 0, 169, 198,
               &lv_font_montserrat_20, UI_INK);
    set_hint("UP PTS              DN CAL");
}

static void render_season_complete(void)
{
    content_reset(UI_PAPER);
    char year[8];
    snprintf(year, sizeof(year), "%u", s_season.year);
    make_label(s_content, year, 0, 4, 198,
               &lv_font_montserrat_14, UI_SKY_DARK);
    make_label(s_content, "SEASON", 0, 48, 198,
               &lv_font_montserrat_20, UI_INK);
    make_label(s_content, "COMPLETE", 0, 78, 198,
               &lv_font_montserrat_20, UI_RED);
    make_label(s_content, "THE FINAL FLAG IS OUT", 0, 125, 198,
               &lv_font_montserrat_14, UI_SKY_DARK);
    make_label(s_content, "CALENDAR + POINTS STAY", 0, 154, 198,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, "AVAILABLE OFFLINE", 0, 177, 198,
               &lv_font_montserrat_14, UI_INK);
    set_hint("UP PTS              DN CAL");
}

static void render_home(void)
{
    if (s_network_state == PDKPASS_NETWORK_SETUP) {
        render_wifi_setup();
        return;
    }
    if (s_state.season_complete) {
        render_season_complete();
        return;
    }

    if (s_state.home_race >= s_season.race_count) {
        render_season_complete();
        return;
    }
    const pdkpass_race_t *race = &s_season.races[s_state.home_race];
    uint32_t ink = contrast_color(race->accent);
    content_reset(UI_PAPER);

    lv_obj_t *round = make_block(s_content, 0, 0, 55, 24, race->accent);
    char round_text[8];
    snprintf(round_text, sizeof(round_text), "R%u", race->round);
    lv_obj_t *round_label = make_label(round, round_text, 0, 2, 55,
                                       &lv_font_montserrat_14, ink);
    lv_obj_set_style_text_align(round_label, LV_TEXT_ALIGN_CENTER, 0);
    make_label(s_content, "NEXT RACE", 64, 2, 130,
               &lv_font_montserrat_14, UI_INK);

    make_label(s_content, race->country, 0, 34, 198,
               &lv_font_montserrat_20, UI_INK);
    make_label(s_content, race->circuit, 1, 61, 198,
               &lv_font_montserrat_14, UI_SKY_DARK);

    lv_obj_t *weekend = make_block(s_content, 0, 86, 198, 31, race->accent);
    char weekend_text[40];
    snprintf(weekend_text, sizeof(weekend_text), "%s  |  %u", race->weekend,
             s_season.year);
    lv_obj_t *weekend_label = make_label(weekend, weekend_text, 0, 6,
                                         198, &lv_font_montserrat_14, ink);
    lv_obj_set_style_text_align(weekend_label, LV_TEXT_ALIGN_CENTER, 0);

    make_label(s_content, race->session_one_cn, 2, 128, 190,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, race->session_two_cn, 2, 151, 190,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, race->race_cn, 2, 174, 190,
               &lv_font_montserrat_14, race->accent);

    set_hint("UP PTS   OK OPEN   DN CAL");
}

static void render_calendar(void)
{
    size_t start = (s_state.selected_race / CALENDAR_ROWS) * CALENDAR_ROWS;
    content_reset(UI_PAPER);

    make_label(s_content, "UPCOMING RACES", 0, 0, 150,
               &lv_font_montserrat_14, UI_INK);
    char year[8];
    snprintf(year, sizeof(year), "%u", s_season.year);
    make_label(s_content, year, 155, 0, 43,
               &lv_font_montserrat_14, UI_SKY_DARK);

    for (size_t row = 0; row < CALENDAR_ROWS; row++) {
        size_t index = start + row;
        if (index >= s_season.race_count) break;
        const pdkpass_race_t *race = &s_season.races[index];
        bool selected = index == s_state.selected_race;
        uint32_t bg = selected ? race->accent : UI_MUTED;
        uint32_t ink = selected ? contrast_color(bg) : UI_INK;
        int y = 27 + (int)row * 32;
        lv_obj_t *card = make_block(s_content, 0, y, 198, 28, bg);

        char round_text[8];
        snprintf(round_text, sizeof(round_text), "R%u", race->round);
        make_label(card, round_text, 5, 4, 32, &lv_font_montserrat_14, ink);
        make_label(card, race->country, 40, 4, 95, &lv_font_montserrat_14, ink);
        lv_obj_t *date = make_label(card, race->weekend, 130, 4, 64,
                                    &lv_font_montserrat_14, ink);
        lv_obj_set_style_text_align(date, LV_TEXT_ALIGN_RIGHT, 0);
    }

    char page[20];
    snprintf(page, sizeof(page), "%u / %u",
             (unsigned)(s_state.selected_race + 1),
             (unsigned)s_season.race_count);
    lv_obj_t *page_label = make_label(s_content, page, 135, 190, 63,
                                      &lv_font_montserrat_14, UI_SKY_DARK);
    lv_obj_set_style_text_align(page_label, LV_TEXT_ALIGN_RIGHT, 0);
    set_hint("UP/DN SELECT  OK OPEN  HOLD HOME");
}

static void render_standings(void)
{
    size_t start = (s_state.selected_driver / STANDINGS_ROWS) * STANDINGS_ROWS;
    content_reset(UI_PAPER);

    make_label(s_content, "DRIVER STANDINGS", 0, 0, 150,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, s_season.standings_as_of, 145, 0, 53,
               &lv_font_montserrat_14, UI_SKY_DARK);

    if (s_season.driver_count == 0U) {
        make_label(s_content, "STANDINGS PENDING", 0, 74, 198,
                   &lv_font_montserrat_20, UI_RED);
        make_label(s_content, "UPDATES AFTER FIRST RACE", 0, 112, 198,
                   &lv_font_montserrat_14, UI_SKY_DARK);
        set_hint("HOLD HOME");
        return;
    }

    for (size_t row = 0; row < STANDINGS_ROWS; row++) {
        size_t index = start + row;
        if (index >= s_season.driver_count) break;
        const pdkpass_driver_t *driver = &s_season.drivers[index];
        bool selected = index == s_state.selected_driver;
        uint32_t bg = selected ? driver->accent : UI_MUTED;
        uint32_t ink = selected ? contrast_color(bg) : UI_INK;
        int y = 27 + (int)row * 27;
        lv_obj_t *card = make_block(s_content, 0, y, 198, 23, bg);

        char position[5];
        char points[8];
        snprintf(position, sizeof(position), "%u", driver->position);
        if (driver->points_tenths % 10U == 0U) {
            snprintf(points, sizeof(points), "%u",
                     driver->points_tenths / 10U);
        } else {
            snprintf(points, sizeof(points), "%u.%u",
                     driver->points_tenths / 10U,
                     driver->points_tenths % 10U);
        }
        make_label(card, position, 5, 2, 22, &lv_font_montserrat_14, ink);
        make_label(card, driver->name, 31, 2, 110, &lv_font_montserrat_14, ink);
        lv_obj_t *points_label = make_label(card, points, 151, 2, 40,
                                            &lv_font_montserrat_14, ink);
        lv_obj_set_style_text_align(points_label, LV_TEXT_ALIGN_RIGHT, 0);
    }

    const pdkpass_driver_t *selected =
        &s_season.drivers[s_state.selected_driver];
    char footer[64];
    snprintf(footer, sizeof(footer), "%s  |  %s", selected->code, selected->team);
    make_label(s_content, footer, 1, 190, 197,
               &lv_font_montserrat_14, UI_SKY_DARK);
    set_hint("UP/DN SCROLL          HOLD HOME");
}

static void render_detail(void)
{
    if (s_state.selected_race >= s_season.race_count) return;
    const pdkpass_race_t *race = &s_season.races[s_state.selected_race];
    uint32_t ink = contrast_color(race->accent);
    content_reset(UI_PAPER);

    lv_obj_t *header = make_block(s_content, 0, 0, 198, 35, race->accent);
    char round_text[12];
    snprintf(round_text, sizeof(round_text), "ROUND %u", race->round);
    make_label(header, round_text, 7, 8, 75, &lv_font_montserrat_14, ink);
    lv_obj_t *weekend = make_label(header, race->weekend, 85, 8, 105,
                                   &lv_font_montserrat_14, ink);
    lv_obj_set_style_text_align(weekend, LV_TEXT_ALIGN_RIGHT, 0);

    make_label(s_content, race->country, 1, 45, 196,
               &lv_font_montserrat_20, UI_INK);
    make_label(s_content, race->circuit, 2, 72, 196,
               &lv_font_montserrat_14, UI_SKY_DARK);

    make_label(s_content, race->session_one_cn, 2, 104, 194,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, race->session_two_cn, 2, 128, 194,
               &lv_font_montserrat_14, UI_INK);
    make_label(s_content, race->race_cn, 2, 152, 194,
               &lv_font_montserrat_14, race->accent);

    char stats[48];
    if (race->circuit_length_m > 0U && race->laps > 0U) {
        snprintf(stats, sizeof(stats), "%u.%03u KM  |  %u LAPS",
                 race->circuit_length_m / 1000,
                 race->circuit_length_m % 1000,
                 race->laps);
    } else {
        snprintf(stats, sizeof(stats), "-- KM  |  -- LAPS");
    }
    make_label(s_content, stats, 2, 181, 194,
               &lv_font_montserrat_14, UI_INK);
    set_hint("UP/DN RACE  OK RESULTS  HOLD BACK");
}

static void render_results(void)
{
    if (s_state.selected_race >= s_season.race_count) return;
    const pdkpass_race_t *race = &s_season.races[s_state.selected_race];
    uint32_t ink = contrast_color(race->accent);
    content_reset(UI_PAPER);

    lv_obj_t *header = make_block(s_content, 0, 0, 198, 35, race->accent);
    char round_text[16];
    snprintf(round_text, sizeof(round_text), "R%u RESULTS", race->round);
    make_label(header, round_text, 7, 8, 105, &lv_font_montserrat_14, ink);
    lv_obj_t *country = make_label(header, race->country, 110, 8, 80,
                                   &lv_font_montserrat_14, ink);
    lv_obj_set_style_text_align(country, LV_TEXT_ALIGN_RIGHT, 0);

    make_label(s_content, pdkpass_session_label(s_state.selected_session),
               1, 44, 196, &lv_font_montserrat_20, UI_INK);

    pdkpass_result_snapshot_t result;
    bool available = pdkpass_results_get(s_state.selected_race,
                                         s_state.selected_session, &result);
    if (!available || result.status != PDKPASS_RESULT_READY) {
        const char *status = "CONNECT TO UPDATE";
        const char *detail = "NO RESULT CACHED";
        if (available && result.status == PDKPASS_RESULT_NOT_HELD) {
            status = "NO SESSION";
            detail = "NOT ON THIS WEEKEND";
        } else if (available && result.status == PDKPASS_RESULT_SCHEDULED) {
            status = "RESULT PENDING";
            detail = "SYNC AFTER SESSION";
        } else if (available && result.status == PDKPASS_RESULT_CANCELLED) {
            status = "SESSION CANCELLED";
            detail = "NO CLASSIFICATION";
        }
        make_label(s_content, status, 1, 91, 196,
                   &lv_font_montserrat_20, race->accent);
        make_label(s_content, detail, 1, 125, 196,
                   &lv_font_montserrat_14, UI_SKY_DARK);
        make_label(s_content, "FREE RESULTS: ABOUT +30 MIN", 1, 171, 196,
                   &lv_font_montserrat_14, UI_INK);
    } else {
        for (size_t i = 0; i < PDKPASS_PODIUM_SIZE; i++) {
            const pdkpass_podium_driver_t *driver = &result.podium[i];
            int y = 70 + (int)i * 40;
            lv_obj_t *row = make_block(s_content, 0, y, 198, 36,
                                       i == 0 ? race->accent : UI_MUTED);
            uint32_t row_ink = i == 0 ? contrast_color(race->accent) : UI_INK;
            char position[4];
            snprintf(position, sizeof(position), "%u", driver->position);
            make_label(row, position, 6, 2, 20,
                       &lv_font_montserrat_20, row_ink);
            make_label(row, driver->code, 34, 2, 42,
                       &lv_font_montserrat_14, row_ink);
            make_label(row, driver->name, 78, 2, 115,
                       &lv_font_montserrat_14, row_ink);
            make_label(row, driver->team, 34, 19, 159,
                       &lv_font_montserrat_14, row_ink);
        }
    }
    set_hint("UP/DN SESSION       OK/HOLD BACK");
}

static void render(void)
{
    switch (s_state.page) {
    case PDKPASS_PAGE_HOME:
        render_home();
        break;
    case PDKPASS_PAGE_CALENDAR:
        render_calendar();
        break;
    case PDKPASS_PAGE_STANDINGS:
        render_standings();
        break;
    case PDKPASS_PAGE_RACE_DETAIL:
        render_detail();
        break;
    case PDKPASS_PAGE_RESULTS:
        render_results();
        break;
    }
}

static void battery_tick(lv_timer_t *timer)
{
    (void)timer;
    int soc = s_battery_available ? bsp_battery_soc() : -1;
    if (soc < 0) {
        lv_label_set_text(s_battery, "BAT --");
        lv_obj_set_style_text_color(s_battery, lv_color_hex(UI_INK), 0);
    } else {
        lv_label_set_text_fmt(s_battery, "BAT %d", soc);
        lv_obj_set_style_text_color(s_battery,
            lv_color_hex(soc < 20 ? UI_RED : UI_INK), 0);
    }
}

static void clock_tick(lv_timer_t *timer)
{
    if (!s_time_valid) return;
    int64_t now = (int64_t)time(NULL);
    size_t previous = s_state.season_complete ? s_season.race_count
                                              : s_state.home_race;
    size_t next = pdkpass_schedule_next_race(now, s_season.races,
                                             s_season.race_count);
    pdkpass_state_set_home_race(&s_state, next, s_season.race_count);
    if (previous != next && s_state.page == PDKPASS_PAGE_HOME) render();

    int64_t deadline = pdkpass_schedule_next_check(now, s_season.races,
                                                   s_season.race_count);
    uint64_t delay_ms = deadline > now ? (uint64_t)(deadline - now) * 1000U
                                       : 1000U;
    if (delay_ms > UINT32_MAX) delay_ms = UINT32_MAX;
    lv_timer_set_period(timer ? timer : s_clock_timer, (uint32_t)delay_ms);
}

static void update_network_label(void)
{
    const char *text = "NET ...";
    uint32_t color = UI_SKY_DARK;
    switch (s_network_state) {
    case PDKPASS_NETWORK_SETUP:
        text = "SETUP";
        color = UI_RED;
        break;
    case PDKPASS_NETWORK_CONNECTING:
        text = "WIFI...";
        break;
    case PDKPASS_NETWORK_SYNCING:
        text = "TIME...";
        break;
    case PDKPASS_NETWORK_ONLINE:
        text = "ONLINE";
        color = 0x00843D;
        break;
    case PDKPASS_NETWORK_OFFLINE:
        text = "OFFLINE";
        color = UI_RED;
        break;
    default:
        break;
    }
    lv_label_set_text(s_network, text);
    lv_obj_set_style_text_color(s_network, lv_color_hex(color), 0);
}

// Save battery without making an unverified claim about GPIO wake support. The
// display dims after 30 seconds and turns off after 90; the next click wakes it.
static void idle_tick(lv_timer_t *timer)
{
    (void)timer;
    s_idle_seconds++;
    if (s_idle_seconds == IDLE_DIM_SECONDS) {
        bsp_display_backlight(25);
        s_idle_stage = 1;
    } else if (s_idle_seconds == IDLE_OFF_SECONDS) {
        bsp_display_backlight(0);
        s_idle_stage = 2;
    }
}

void pdkpass_ui_enter(bool battery_available)
{
    s_battery_available = battery_available;
    s_idle_seconds = 0;
    s_idle_stage = 0;
    pdkpass_state_init(&s_state);
    if (!pdkpass_season_snapshot(&s_season)) memset(&s_season, 0, sizeof(s_season));

    s_screen = ui_pixel_screen_create("PDKPASS");
    s_content = ui_pixel_panel_create(s_screen, CONTENT_X, CONTENT_Y,
                                       CONTENT_W, CONTENT_H, UI_PAPER);
    s_network = make_label(s_screen, "NET ...", 96, 28, 64,
                           &lv_font_montserrat_14, UI_SKY_DARK);
    s_battery = make_label(s_screen, "BAT --", 163, 28, 65,
                           &lv_font_montserrat_14, UI_INK);
    s_hint = make_label(s_screen, "", 8, 268, 184,
                        &lv_font_montserrat_14, UI_INK);
    s_mascot = ui_pixel_mascot_create(s_screen, 194, 238);

    render();
    battery_tick(NULL);
    s_battery_timer = lv_timer_create(battery_tick, 60000, NULL);
    s_idle_timer = lv_timer_create(idle_tick, 1000, NULL);
    s_clock_timer = lv_timer_create(clock_tick, CLOCK_FALLBACK_PERIOD_MS, NULL);
    lv_screen_load(s_screen);
}

void pdkpass_ui_network_update(const pdkpass_network_update_t *update)
{
    if (!update) return;
    pdkpass_network_state_t previous_state = s_network_state;
    s_network_state = update->state;
    s_time_valid = update->time_valid;
    snprintf(s_setup_ssid, sizeof(s_setup_ssid), "%s",
             update->setup_ssid ? update->setup_ssid : "");
    snprintf(s_setup_password, sizeof(s_setup_password), "%s",
             update->setup_password ? update->setup_password : "");
    update_network_label();
    clock_tick(NULL);
    if (previous_state != s_network_state &&
        s_state.page == PDKPASS_PAGE_HOME) render();
}

void pdkpass_ui_results_update(size_t race_index)
{
    if (s_state.page == PDKPASS_PAGE_RESULTS &&
        s_state.selected_race == race_index) render();
}

void pdkpass_ui_season_update(void)
{
    pdkpass_season_snapshot_t updated;
    if (!pdkpass_season_snapshot(&updated)) return;
    s_season = updated;
    if (s_state.selected_race >= s_season.race_count) {
        s_state.selected_race = s_season.race_count > 0U
                                    ? s_season.race_count - 1U
                                    : 0U;
    }
    if (s_state.selected_driver >= s_season.driver_count) {
        s_state.selected_driver = s_season.driver_count > 0U
                                      ? s_season.driver_count - 1U
                                      : 0U;
    }
    if (s_time_valid) {
        int64_t now = (int64_t)time(NULL);
        size_t next = pdkpass_schedule_next_race(now, s_season.races,
                                                 s_season.race_count);
        pdkpass_state_set_home_race(&s_state, next, s_season.race_count);
    }
    render();
}

void pdkpass_ui_key(bsp_btn_t btn, bsp_btn_ev_t ev)
{
    bool was_off = s_idle_stage == 2;
    if (ev == BSP_BTN_CLICK || ev == BSP_BTN_LONG) {
        s_idle_seconds = 0;
        s_idle_stage = 0;
        bsp_display_backlight(100);
    }
    if (was_off) return;

    pdkpass_input_t input;
    if (btn == BSP_BTN_OK && ev == BSP_BTN_LONG) {
        input = PDKPASS_INPUT_BACK;
    } else if (ev != BSP_BTN_CLICK) {
        return;
    } else if (btn == BSP_BTN_UP) {
        input = PDKPASS_INPUT_UP;
    } else if (btn == BSP_BTN_DOWN) {
        input = PDKPASS_INPUT_DOWN;
    } else {
        input = PDKPASS_INPUT_OK;
    }

    pdkpass_state_handle(&s_state, input,
                         s_season.race_count, s_season.driver_count);
    render();
    if (s_state.page == PDKPASS_PAGE_RESULTS) {
        pdkpass_results_request_race(s_state.selected_race);
    }
    ui_pixel_mascot_jump(s_mascot);
}
