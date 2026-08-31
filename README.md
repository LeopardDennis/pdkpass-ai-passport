<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# PDKPASS

PDKPASS is an offline-first Formula 1 weekend companion for FoloToy AI Passport.
It turns the 240 × 320 wearable display into a glanceable race calendar, driver
standings board, and race-detail pass.

## First release

- Boots directly into the next-race dashboard.
- Browses the remaining 2026 calendar with a race-specific accent colour.
- Shows the complete 2026 driver standings snapshot from 31 August 2026.
- Opens race details with CST session times, circuit length, and lap count.
- Runs fully offline and dims/turns off the backlight after inactivity.
- Preserves AI Passport mini-program installation, protected `cardid`, permanent
  Recovery, and the five-second UP-key Recovery gesture.

## Controls

| Screen | UP / DOWN | OK | Hold OK |
| --- | --- | --- | --- |
| Next race | Standings / calendar | Race details | — |
| Calendar | Select race | Race details | Home |
| Standings | Scroll drivers | — | Home |
| Race details | Previous / next race | Back | Back |

## Data snapshot

The bundled data is an offline snapshot captured on 31 August 2026 from the
[official 2026 Formula 1 calendar](https://www.formula1.com/en/racing/2026) and
[official driver standings](https://www.formula1.com/en/results/2026/drivers).
Displayed session times are converted to China Standard Time (UTC+8).

PDKPASS is an independent fan project and is not affiliated with or endorsed by
Formula 1, the FIA, or FoloToy. Formula 1 and related marks belong to their
respective owners.

## Build

Use ESP-IDF 5.5.3 and run:

```bash
./tools/validate.sh --static
./tools/validate.sh --firmware
```

Only distribute the validated `build/FoloToy-AI-Passport-full.bin` artifact.
