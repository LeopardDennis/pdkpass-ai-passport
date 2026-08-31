#include "pdkpass_ui.h"

#include "bsp_battery.h"
#include "bsp_display.h"
#include "pdkpass_data.h"
#include "pdkpass_model.h"
#include "ui_pixel.h"
#include "lvgl.h"
#include <stdio.h>

#define CONTENT_X 10
#define CONTENT_Y 53
#define CONTENT_W 220
#define CONTENT_H 211
#define CALENDAR_ROWS 5
#define STANDINGS_ROWS 6
#define IDLE_DIM_SECONDS 30
#define IDLE_OFF_SECONDS 90

static lv_obj_t *s_screen;
static lv_obj_t *s_content;
static lv_obj_t *s_hint;
static lv_obj_t *s_battery;
static lv_obj_t *s_mascot;
static lv_timer_t *s_battery_timer;
static lv_timer_t *s_idle_timer;
static pdkpass_state_t s_state;
static bool s_battery_available;
static unsigned s_idle_seconds;
static unsigned s_idle_stage;

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

static void render_home(void)
{
    const pdkpass_race_t *race = &pdkpass_races[0];
    uint32_t ink = contrast_color(race->accent);
    content_reset(UI_PAPER);

    lv_obj_t *round = make_block(s_content, 0, 0, 55, 24, race->accent);
    lv_obj_t *round_label = make_label(round, "R13", 0, 2, 55,
                                       &lv_font_montserrat_14, ink);
    lv_obj_set_style_text_align(round_label, LV_TEXT_ALIGN_CENTER, 0);
    make_label(s_content, "NEXT RACE", 64, 2, 130,
               &lv_font_montserrat_14, UI_INK);

    make_label(s_content, race->country, 0, 34, 198,
               &lv_font_montserrat_20, UI_INK);
    make_label(s_content, race->circuit, 1, 61, 198,
               &lv_font_montserrat_14, UI_SKY_DARK);

    lv_obj_t *weekend = make_block(s_content, 0, 86, 198, 31, race->accent);
    lv_obj_t *weekend_label = make_label(weekend, "04-06 SEP  |  2026", 0, 6,
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
    make_label(s_content, "2026", 155, 0, 43,
               &lv_font_montserrat_14, UI_SKY_DARK);

    for (size_t row = 0; row < CALENDAR_ROWS; row++) {
        size_t index = start + row;
        if (index >= pdkpass_race_count) break;
        const pdkpass_race_t *race = &pdkpass_races[index];
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
             (unsigned)pdkpass_race_count);
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
    make_label(s_content, "31 AUG", 150, 0, 48,
               &lv_font_montserrat_14, UI_SKY_DARK);

    for (size_t row = 0; row < STANDINGS_ROWS; row++) {
        size_t index = start + row;
        if (index >= pdkpass_driver_count) break;
        const pdkpass_driver_t *driver = &pdkpass_drivers[index];
        bool selected = index == s_state.selected_driver;
        uint32_t bg = selected ? driver->accent : UI_MUTED;
        uint32_t ink = selected ? contrast_color(bg) : UI_INK;
        int y = 27 + (int)row * 27;
        lv_obj_t *card = make_block(s_content, 0, y, 198, 23, bg);

        char position[5];
        char points[8];
        snprintf(position, sizeof(position), "%u", driver->position);
        snprintf(points, sizeof(points), "%u", driver->points);
        make_label(card, position, 5, 2, 22, &lv_font_montserrat_14, ink);
        make_label(card, driver->name, 31, 2, 110, &lv_font_montserrat_14, ink);
        lv_obj_t *points_label = make_label(card, points, 151, 2, 40,
                                            &lv_font_montserrat_14, ink);
        lv_obj_set_style_text_align(points_label, LV_TEXT_ALIGN_RIGHT, 0);
    }

    const pdkpass_driver_t *selected = &pdkpass_drivers[s_state.selected_driver];
    char footer[64];
    snprintf(footer, sizeof(footer), "%s  |  %s", selected->code, selected->team);
    make_label(s_content, footer, 1, 190, 197,
               &lv_font_montserrat_14, UI_SKY_DARK);
    set_hint("UP/DN SCROLL          HOLD HOME");
}

static void render_detail(void)
{
    const pdkpass_race_t *race = &pdkpass_races[s_state.selected_race];
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
    snprintf(stats, sizeof(stats), "%u.%03u KM  |  %u LAPS",
             race->circuit_length_m / 1000,
             race->circuit_length_m % 1000,
             race->laps);
    make_label(s_content, stats, 2, 181, 194,
               &lv_font_montserrat_14, UI_INK);
    set_hint("UP/DN RACE   OK/HOLD BACK");
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

    s_screen = ui_pixel_screen_create("PDKPASS");
    s_content = ui_pixel_panel_create(s_screen, CONTENT_X, CONTENT_Y,
                                       CONTENT_W, CONTENT_H, UI_PAPER);
    s_battery = make_label(s_screen, "BAT --", 163, 28, 65,
                           &lv_font_montserrat_14, UI_INK);
    s_hint = make_label(s_screen, "", 8, 268, 184,
                        &lv_font_montserrat_14, UI_INK);
    s_mascot = ui_pixel_mascot_create(s_screen, 194, 238);

    render();
    battery_tick(NULL);
    s_battery_timer = lv_timer_create(battery_tick, 60000, NULL);
    s_idle_timer = lv_timer_create(idle_tick, 1000, NULL);
    lv_screen_load(s_screen);
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
                         pdkpass_race_count, pdkpass_driver_count);
    render();
    ui_pixel_mascot_jump(s_mascot);
}
