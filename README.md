<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# PDKPASS

PDKPASS is an offline-first Formula 1 weekend companion for FoloToy AI Passport
with optional network time synchronization.
It turns the 240 × 320 wearable display into a glanceable race calendar, driver
standings board, and race-detail pass.

## First release

- Uses Beijing time to open the current or next-race dashboard automatically.
- Browses the remaining 2026 calendar with a race-specific accent colour.
- Shows the complete 2026 driver standings snapshot from 31 August 2026.
- Opens race details with CST session times, circuit length, and lap count.
- Keeps the calendar and standings available offline, and dims/turns off the
  backlight after inactivity.
- Preserves AI Passport mini-program installation, protected `cardid`, permanent
  Recovery, and the five-second UP-key Recovery gesture.

## Controls

| Screen | UP / DOWN | OK | Hold OK |
| --- | --- | --- | --- |
| Next race | Standings / calendar | Race details | — |
| Calendar | Select race | Race details | Home |
| Standings | Scroll drivers | — | Home |
| Race details | Previous / next race | Back | Back |

## First-time Wi-Fi setup

No phone app is required. When no working network has been saved, PDKPASS shows
a temporary Wi-Fi name, password, and `192.168.4.1` on its home screen:

1. Connect a phone to the displayed `PDKPASS-XXXX` Wi-Fi network.
2. Open `http://192.168.4.1` in the phone browser.
3. Enter a 2.4 GHz Wi-Fi name and password, then press **Connect**.

PDKPASS tests the connection before saving it. A wrong password leaves the setup
page available for another attempt. After five failed attempts to reconnect to a
previously saved network, setup becomes available again automatically.

The top status changes through `SETUP`, `WIFI...`, `TIME...`, and `ONLINE`.
After time synchronization, the device keeps counting locally and rechecks the
home race at Beijing midnight and at the current round's switch boundary. Each
round remains current until four hours after its scheduled race start, then the
dashboard advances to the following round. After the final round it displays
`SEASON COMPLETE`.

## Data snapshot

The bundled data is an offline snapshot captured on 31 August 2026 from the
[official 2026 Formula 1 calendar](https://www.formula1.com/en/racing/2026) and
[official driver standings](https://www.formula1.com/en/results/2026/drivers).
Displayed session times are converted to China Standard Time (UTC+8).
The network connection only synchronizes time in this release; it does not
download new calendar or standings data. The most recent valid time is saved as
an offline fallback. After a long powered-off period, reconnect to refresh it.

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
