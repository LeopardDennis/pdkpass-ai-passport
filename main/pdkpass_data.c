#include "pdkpass_data.h"

// Offline snapshot captured on 2026-08-31 from the official Formula 1 calendar
// and driver standings. Times below are converted to China Standard Time (UTC+8).
// Schedule: https://www.formula1.com/en/racing/2026
// Standings: https://www.formula1.com/en/results/2026/drivers
const pdkpass_race_t pdkpass_races[] = {
    { 13, "ITALY",      "MONZA",        "04-06 SEP", "FP1   04 SEP 18:30", "QUALI 05 SEP 22:00", "RACE  06 SEP 21:00", 5793, 53, 0xE32636, 1788714000 },
    { 14, "SPAIN",      "MADRING",      "11-13 SEP", "FP1   11 SEP 19:30", "QUALI 12 SEP 22:00", "RACE  13 SEP 21:00", 5416, 57, 0xF2A900, 1789318800 },
    { 15, "AZERBAIJAN", "BAKU",         "24-26 SEP", "FP1   24 SEP 16:30", "QUALI 25 SEP 20:00", "RACE  26 SEP 19:00", 6003, 51, 0x00A6C8, 1790434800 },
    { 16, "MALAYSIA",   "SEPANG",       "02-04 OCT", "FP1   02 OCT 12:30", "QUALI 03 OCT 16:00", "RACE  04 OCT 15:00", 5543, 56, 0xFF7A00, 1791111600 },
    { 17, "SINGAPORE",  "MARINA BAY",   "09-11 OCT", "FP1   09 OCT 16:30", "SPR Q 09 OCT 20:30", "RACE  11 OCT 20:00", 4927, 62, 0x8A3FFC, 1791734400 },
    { 18, "USA",        "COTA",         "23-25 OCT", "FP1   24 OCT 01:30", "QUALI 25 OCT 05:00", "RACE  26 OCT 04:00", 5513, 56, 0x0057B8, 1792972800 },
    { 19, "MEXICO",     "MEXICO CITY",  "30 OCT-01 NOV", "FP1   31 OCT 02:30", "QUALI 01 NOV 05:00", "RACE  02 NOV 04:00", 4304, 71, 0x00843D, 1793577600 },
    { 20, "BRAZIL",     "INTERLAGOS",   "06-08 NOV", "FP1   06 NOV 23:30", "QUALI 08 NOV 02:00", "RACE  09 NOV 01:00", 4309, 71, 0xFFCC29, 1794171600 },
    { 21, "LAS VEGAS",  "LAS VEGAS",    "19-21 NOV", "FP1   20 NOV 08:30", "QUALI 21 NOV 12:00", "RACE  22 NOV 12:00", 6201, 50, 0xD3208B, 1795334400 },
    { 22, "QATAR",      "LUSAIL",       "27-29 NOV", "FP1   27 NOV 21:30", "QUALI 29 NOV 02:00", "RACE  30 NOV 00:00", 5419, 57, 0x8A1538, 1795982400 },
    { 23, "ABU DHABI",  "YAS MARINA",   "04-06 DEC", "FP1   04 DEC 17:30", "QUALI 05 DEC 22:00", "RACE  06 DEC 21:00", 5281, 58, 0x00A9A5, 1796576400 },
};

const size_t pdkpass_race_count = sizeof(pdkpass_races) / sizeof(pdkpass_races[0]);

const pdkpass_driver_t pdkpass_drivers[] = {
    {  1, "ANT", "ANTONELLI",  "MERCEDES",     242, 0x00A19C },
    {  2, "RUS", "RUSSELL",    "MERCEDES",     183, 0x00A19C },
    {  3, "HAM", "HAMILTON",   "FERRARI",      183, 0xE32636 },
    {  4, "NOR", "NORRIS",     "MCLAREN",      159, 0xFF8700 },
    {  5, "LEC", "LECLERC",    "FERRARI",      155, 0xE32636 },
    {  6, "VER", "VERSTAPPEN", "RED BULL",     112, 0x3671C6 },
    {  7, "PIA", "PIASTRI",    "MCLAREN",      104, 0xFF8700 },
    {  8, "HAD", "HADJAR",     "RED BULL",      68, 0x3671C6 },
    {  9, "LAW", "LAWSON",     "RED BULL",      49, 0x3671C6 },
    { 10, "GAS", "GASLY",      "ALPINE",        44, 0x2293D1 },
    { 11, "LIN", "LINDBLAD",   "RACING BULLS",  23, 0x6692FF },
    { 12, "COL", "COLAPINTO",  "ALPINE",        19, 0x2293D1 },
    { 13, "BEA", "BEARMAN",    "HAAS",          18, 0xB6BABD },
    { 14, "BOR", "BORTOLETO",  "AUDI",          10, 0xF50537 },
    { 15, "HUL", "HULKENBERG", "AUDI",           6, 0xF50537 },
    { 16, "SAI", "SAINZ",      "WILLIAMS",        6, 0x64C4FF },
    { 17, "ALB", "ALBON",      "WILLIAMS",        5, 0x64C4FF },
    { 18, "OCO", "OCON",       "HAAS",            3, 0xB6BABD },
    { 19, "ALO", "ALONSO",     "ASTON MARTIN",    3, 0x229971 },
    { 20, "TSU", "TSUNODA",    "RACING BULLS",    0, 0x6692FF },
    { 21, "STR", "STROLL",     "ASTON MARTIN",    0, 0x229971 },
    { 22, "BOT", "BOTTAS",     "CADILLAC",        0, 0x1B2D57 },
    { 23, "PER", "PEREZ",      "CADILLAC",        0, 0x1B2D57 },
};

const size_t pdkpass_driver_count = sizeof(pdkpass_drivers) / sizeof(pdkpass_drivers[0]);
