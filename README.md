<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# PDKPASS

PDKPASS is an offline-first Formula 1 weekend companion for FoloToy AI Passport
with automatic online season updates and network time synchronization.
It turns the 240 × 320 wearable display into a glanceable race calendar, driver
standings board, race-detail pass, and session-podium archive.

## First release

- Uses Beijing time to open the current or next-race dashboard automatically.
- Downloads and caches the current season calendar with a race-specific accent
  colour, while retaining a bundled 2026 fallback for first use without Wi-Fi.
- Updates the driver standings from the latest completed race and keeps them
  available offline.
- Opens race details with CST session times and, when known, circuit length and
  lap count.
- Downloads and caches the top three from practice, sprint qualifying, sprint,
  qualifying, and race sessions after the free historical-results delay.
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
| Race details | Previous / next race | Session results | Back |
| Session results | Previous / next session | Race details | Race details |

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
After time synchronization, the device keeps counting locally. It rechecks the
season at Beijing midnight and at the current round's switch boundary, as well
as after the network becomes available. Each online round remains current until
its recorded race end, then the dashboard advances to the following round. The
bundled offline fallback uses a four-hour window from the scheduled race start.
After the final round it displays `SEASON COMPLETE`.

## Session results

When a session has ended, open its race details and press **OK** to browse FP1,
FP2, FP3, sprint qualifying, sprint, qualifying, and race results. PDKPASS waits
at least 30 minutes after the recorded session end, then checks for the top
three. The background worker retries at a low rate, so a free result normally
appears about 30–40 minutes after the session and remains available offline once
cached. A normal weekend reports `NO SESSION` for sprint-only slots.

Historical session classifications and driver metadata come from the unofficial
[OpenF1 API](https://openf1.org/docs/). PDKPASS uses the unauthenticated
historical endpoint and does not embed an OpenF1 account, password, or access
token.

## Season data and offline behaviour

The first-use fallback is an offline snapshot captured on 31 August 2026 from the
[official 2026 Formula 1 calendar](https://www.formula1.com/en/racing/2026) and
[official driver standings](https://www.formula1.com/en/results/2026/drivers).
Displayed session times are converted to China Standard Time (UTC+8).

After the first successful connection, PDKPASS downloads and stores the Grand
Prix calendar for the current Beijing-time year and the standings from the
latest completed race. At a year boundary it switches only after OpenF1 returns
at least one valid Grand Prix for the new year; otherwise it keeps the last
working season and tries again at the next scheduled check. The device stores
one current season at a time, including up to 24 races, 24 drivers, and the
cached podiums for all seven supported session types. The previous season cache
is replaced only after the new season has been accepted.

The most recent valid time, accepted season, standings, and downloaded podiums
remain available offline. After a long powered-off period, reconnect to refresh
them. Dynamic calendar, standings, session classifications, and driver metadata
come from the unofficial [OpenF1 API](https://openf1.org/docs/).

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
