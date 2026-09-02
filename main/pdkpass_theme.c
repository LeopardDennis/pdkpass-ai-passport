#include "pdkpass_theme.h"

#include <stddef.h>
#include <string.h>

typedef struct {
    const char *circuit;
    uint32_t top;
} theme_entry_t;

// Each 2026 venue owns a distinct background colour. These are deliberately
// independent of race accents, which continue to identify cards and results.
static const theme_entry_t s_themes[] = {
    { "MELBOURNE",       0x0871CD },
    { "SHANGHAI",        0xC8102E },
    { "SUZUKA",          0xD84A7F },
    { "MIAMI",           0xD5007F },
    { "MONTREAL",        0xD52B1E },
    { "MONACO",          0x00A6C8 },
    { "BARCELONA",       0xFF7A00 },
    { "SPIELBERG",       0xB50925 },
    { "SILVERSTONE",     0x1D4ED8 },
    { "SPA-FRANCORCHAMPS", 0x365C7D },
    { "HUNGARORING",     0x7B2CBF },
    { "ZANDVOORT",       0xE85D04 },
    { "MONZA",           0x59636E },
    { "MADRING",         0x5B3FD6 },
    { "BAKU",            0x009CA6 },
    { "SEPANG",          0x009B77 },
    { "MARINA BAY",      0x541388 },
    { "COTA",            0x173F5F },
    { "MEXICO CITY",     0x00843D },
    { "INTERLAGOS",      0x4C9F38 },
    { "LAS VEGAS",       0xFF2DAA },
    { "LUSAIL",          0x8A1538 },
    { "YAS MARINA",      0x004C6D },
};

static uint32_t darken(uint32_t color)
{
    const uint32_t factor = 222U;
    uint32_t red = ((color >> 16) & 0xffU) * factor / 256U;
    uint32_t green = ((color >> 8) & 0xffU) * factor / 256U;
    uint32_t blue = (color & 0xffU) * factor / 256U;
    return (red << 16) | (green << 8) | blue;
}

bool pdkpass_theme_get(const char *circuit, pdkpass_theme_t *theme)
{
    if (!circuit || !theme) return false;
    const char *lookup = strcmp(circuit, "MADRID") == 0 ? "MADRING" : circuit;
    for (size_t i = 0; i < sizeof(s_themes) / sizeof(s_themes[0]); i++) {
        if (strcmp(lookup, s_themes[i].circuit) == 0) {
            theme->top = s_themes[i].top;
            theme->bottom = darken(theme->top);
            return true;
        }
    }
    return false;
}
