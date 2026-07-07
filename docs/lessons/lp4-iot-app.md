---
title: "LP4 - ESP8266 IoT App Setup"
description: "Set up the Arduino IDE for the ESP8266 (NodeMCU), serve a small web app, use an AI coding agent through an external editor, and pair it with a FRDM-MCXA153 sensor node over UART"
nav_order: 5
parent: Additional Demos
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src_arduino/simple_server_esp8266
---

# LP4 - ESP8266 IoT App Setup

**Configure the Arduino IDE for an ESP8266 NodeMCU board, upload a small Wi-Fi web app, and iterate on it with an AI coding agent in an external editor.**

---

| | |
|---|---|
| **Session** | IoT / Wi-Fi demo, separate from the FRDM-MCXA153 track |
| **Board** | ESP8266 NodeMCU 1.0 (ESP-12E Module) |
| **Tooling** | Arduino IDE 1.8.x, ESP8266 Boards package, external editor (VS Code) + AI coding agent |
| **Console** | Serial Monitor, 115200 baud |
| **Local project** | `src_arduino/simple_server_esp8266` |
| **Sensor node project** | `src/lab_new_project_configured/configured_iot` (FRDM-MCXA153 + P3T1755) |
| **Main deliverable** | ESP8266 sketch that connects to Wi-Fi, serves an HTML page, and exposes a `/value` JSON endpoint updated over UART |

## Context

This lab is a self-contained detour from the FRDM-MCXA153 track: an ESP8266 NodeMCU board running the Arduino core, used to build a tiny IoT web app. The board joins a Wi-Fi network, serves a web page over HTTP, and exposes the current numeric value as JSON at `/value`. The value itself is updated over UART with a newline-terminated JSON message, so another board (or a PC script) can push data into the ESP8266 to display.

The setup steps below cover installing ESP8266 board support in the Arduino IDE, selecting the right board and upload settings, and using an external editor with an AI coding agent to iterate on the sketch instead of the built-in Arduino editor.

## Objectives

By the end of this lab you should be able to:

1. install ESP8266 board support in the Arduino IDE;
2. select the correct board, upload speed, and flash size for a NodeMCU 1.0 (ESP-12E) module;
3. build and upload the sketch, and confirm success from the upload log;
4. switch to an external editor and use an AI coding agent to add a feature to the sketch;
5. pair the ESP8266 with a FRDM-MCXA153 sensor node so the board sends live P3T1755 temperature readings over UART, using the spec-then-codegen agent workflow instead of hand-written code;
6. use the feedback-to-change-spec agent to diagnose a runtime observation (the ESP8266 link is not visible on the console) and turn it into a small, targeted change instead of guessing at code;
7. extend the web page with an AI coding agent to chart the temperature reading over time instead of only showing the latest value.

## Setup Steps

### 1. Install ESP8266 Board Support

Open **File -> Preferences** and add the ESP8266 boards index URL to **Additional Boards Manager URLs**:

```text
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

If you already have another URL there (for example the ESP32 index), separate the two with a comma. Then open **Tools -> Board -> Boards Manager**, search for `esp8266`, and install the package.

### 2. Select the Board

Open **Tools -> Board -> ESP8266 Boards** and select **NodeMCU 1.0 (ESP-12E Module)**.

![Tools > Board > ESP8266 Boards - selecting NodeMCU 1.0 (ESP-12E Module)](../img/labp4_iot_app/1_select_board.png)

### 3. Configure Board Settings

With the ESP8266 board selected, the **Tools** menu grows extra entries. Set at least:

| Setting | Value |
|---|---|
| Board | `NodeMCU 1.0 (ESP-12E Module)` |
| Upload Speed | `921600` |
| CPU Frequency | `80 MHz` |
| Flash Size | `4MB (FS:2MB OTA:~1019KB)` |
| Port | the COM port the board enumerates on (for example `COM8`) |

![Tools menu - upload speed, flash size, and other NodeMCU settings](../img/labp4_iot_app/2_board_settings_and_upload.png)

A successful upload ends with a log similar to:

```text
Writing at 0x00038000... (93 %)
Writing at 0x0003c000... (100 %)
Wrote 338784 bytes (246291 compressed) at 0x00000000 in 5.8 seconds (effective 470.6 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```

Open **Tools -> Serial Monitor** at `115200` baud to see the board's Wi-Fi status and IP address after reset.

### 4. Use an External Editor with an AI Coding Agent

The built-in Arduino editor has no AI assistant. Enable **File -> Preferences -> Use external editor**, then open the sketch folder in an editor such as VS Code. Arduino IDE keeps compiling and uploading; the external editor (with an AI coding agent) is only used to edit files.

![Arduino IDE Preferences - Use external editor enabled](../img/labp4_iot_app/3_external_editor_preferences.png)

With the sketch open in VS Code, use an AI coding agent to add or change features. For example, asking the agent to change the UART protocol to JSON:

![AI coding agent editing simple_server_esp8266.ino to add a JSON UART protocol](../img/labp4_iot_app/4_ai_agent_feature_edit.png)

Save the file in the external editor, then switch back to Arduino IDE to verify and upload — Arduino IDE reloads the sketch from disk automatically.

## Pairing with a FRDM-MCXA153 Sensor Node

The ESP8266 only displays a number; it does not measure anything itself. To turn this into a two-board IoT demo, pair it with a FRDM-MCXA153 running `src/lab_new_project_configured/configured_iot`, which already reads a P3T1755 temperature sensor over `LPI2C0`. The NXP board becomes the sensor node and the ESP8266 becomes the Wi-Fi gateway: the NXP board pushes each reading over a wired UART link, and the ESP8266 forwards it to `/value` and the web page.

### Wiring

The FRDM-MCXA153 side uses two separate UART instances so the ESP8266 link and the local debug console don't share a wire: `LPUART0` (header pins `RX0`/`TX0`) stays the console, and a second instance — `LPUART2` in this project, added in MCUXpresso Config Tools and routed to a pair of spare pins — is dedicated to the ESP8266 link. Connect the two boards' UARTs directly, crossed, with a common ground:

| FRDM-MCXA153 (`LPUART2`, pins `62`/`63`) | ESP8266 NodeMCU |
|---|---|
| `TX2` (`P1_5`, pin `63`) | `RXD` (`Serial` RX) |
| `RX2` (`P1_4`, pin `62`) | `TXD` (`Serial` TX) |
| `GND` | `GND` |

Both sides are configured for `115200 8N1`: `LPUART2_config` in the generated `peripherals.c` on the NXP side, and `Serial.begin(115200)` in the ESP8266 sketch. `LPUART0` keeps its own `115200 8N1` config and its `RX0`/`TX0` header pins free for a Serial Monitor / terminal on the PC.

If you rename or renumber the LPUART instance in Config Tools later (for example moving from `LPUART1` to `LPUART2` because of a pin conflict), regenerate the project first and then ask the codegen agent to update every reference in `app/uart.c`/`app/uart.h`/the root C file to the new instance — don't hand-edit the generated macros.

### Protocol

The ESP8266 sketch reads newline-terminated lines on `Serial` and only accepts a line that parses as JSON with `"code":"data"` and a numeric `"value"`. On success it replies `{"code":"ack"}`. So the NXP board must write lines like:

```json
{"code":"data","value":21.50}
```

The NXP side does not need to read the ack for the protocol to work, but so the whole exchange is observable from a single Serial Monitor, the NXP application also:

- mirrors every `{"code":"data",...}` line it sends on `LPUART2` onto `LPUART0` (the console), and
- bridges raw bytes both ways between `LPUART0` and `LPUART2` on every main-loop iteration, so the ESP8266's `{"code":"ack"}` reply (received on `LPUART2`) is echoed live onto the console, and anything typed into the console is forwarded to the ESP8266.

This bridge is a simple byte-for-byte relay with no framing, so avoid typing into the console while a data line is in flight — it can interleave with the JSON on the wire to the ESP8266.

### Implement the NXP Side With the Spec-Then-Codegen Agents

`configured_iot/.agents/` already has the three reusable prompts described in [From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent): `mcux-config-to-main-spec.prompt.md`, `mcux-main-codegen-from-spec.prompt.md`, and `mcux-feedback-to-change-spec.prompt.md`. Use that workflow instead of hand-writing the UART code, so the agent verifies the P3T1755/LPUART configuration before generating anything.

Before running the spec agent, add a second LPUART instance in MCUXpresso Config Tools (`LPUART2` here) routed to a spare pair of pins, `115200 8N1`, RX and TX both enabled, and regenerate the project. This keeps `LPUART0` free to stay the console instead of overloading one wire with both the human-readable trace and the machine-readable ESP8266 protocol.

<details class="prompt-code" markdown="1">
<summary><strong>Step 1: Generate the Specification</strong></summary>

<div markdown="1">

````text
Use .agents/mcux-config-to-main-spec.prompt.md.

APPLICATION_TASK:
Send the P3T1755 temperature reading to a separate ESP8266 board over UART, once per existing print tick.
Reuse the existing P3T1755_ReadTemperatureRaw() reading; do not add a second sensor read path.
Send it on LPUART2, a second UART instance already configured at 115200 8N1 with RX/TX enabled and routed to its own pins, kept separate from LPUART0 (the debug console).
The message must be a single line, terminated by '\n', of the exact form:
{"code":"data","value":21.50}
where "value" is the temperature in Celsius as a plain JSON number with 2 fractional digits (matches the Q4 sensor resolution) and a leading '-' only when negative.
This exact format is required because a separate ESP8266 sketch reads newline-terminated JSON lines on this UART and only accepts a line with "code":"data" and a numeric "value"; it replies with {"code":"ack"} on success. Reading that reply is optional.
If the P3T1755 read fails for a tick, do not send a malformed "data" line for that tick.
Keep the existing ADC sampling and PWM LED fade behavior on every SysTick_Handler tick unchanged.
Reuse app/uart.c for the writes; add small formatting helpers there instead of inlining LPUART_WriteBlocking calls in led_blinky.c.

Use the current working directory as PROJECT_ROOT.

Return the specification only. Do not edit files.
````

</div>
</details>

<details class="prompt-code" markdown="1">
<summary><strong>Step 2: Generate the Code From the Specification</strong></summary>

<div markdown="1">

````text
Use .agents/mcux-main-codegen-from-spec.prompt.md with the specification above.

Use the current working directory as PROJECT_ROOT.
Apply the code changes only to the active root C file and app/uart.c.
Do not edit .mex or generated board files.
````

</div>
</details>

If the specification comes back `BLOCKED` (for example, if `LPUART2` were not routed to external pins), fix the configuration in MCUXpresso Config Tools and regenerate before trying again — do not let the agent invent pins or peripherals.

### Iterating With the Feedback-To-Change-Spec Agent

Once the two boards are talking, a natural follow-up observation is: *"it sends the temperature all right, it arrives on the ESP8266, but I don't see the data in the console (Serial Monitor)."* That is a runtime observation, not a new feature request, so instead of guessing at a fix, feed it to `mcux-feedback-to-change-spec.prompt.md`:

<details class="prompt-code" markdown="1">
<summary><strong>Step 3: Turn a Runtime Observation Into a Change Specification</strong></summary>

<div markdown="1">

````text
Use .agents/mcux-feedback-to-change-spec.prompt.md.

FEEDBACK:
It sends the temperature all right, it arrives on the ESP8266 device, but I don't see the data in the console (Serial Monitor).

INTENDED_BEHAVIOR:
The temperature JSON line sent to the ESP8266 over LPUART2, and the ESP8266's {"code":"ack"} reply, should both be visible on the LPUART0 console.

Use the current working directory as PROJECT_ROOT.

Return the specification only. Do not edit files.
````

</div>
</details>

The agent traces through the generated config and the application code to confirm the actual cause: the JSON line is only ever written to `LPUART2`, and `LPUART0` is a separate, otherwise-idle peripheral, so nothing about the ESP8266 exchange was ever going out on the wire the console is watching. The resulting change specification is small and targeted — mirror the outgoing JSON line onto `LPUART0`, and add a byte-for-byte bridge between `LPUART0` and `LPUART2` so the ESP8266's reply becomes visible too — and it feeds straight back into `mcux-main-codegen-from-spec.prompt.md` (Step 2) to implement. This is the same three-prompt loop as [From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent): spec, codegen, then feedback-to-change-spec for anything the first pass didn't get right.

### Bring-Up

1. Flash `configured_iot` to the FRDM-MCXA153 and `simple_server_esp8266` to the ESP8266.
2. Wire `TX2`/`RX2`/`GND` between the two boards as above, crossed.
3. Open a Serial Monitor / terminal on the FRDM-MCXA153's `LPUART0` console: each tick should print `{"code":"data","value":...}`, followed shortly by `{"code":"ack"}` echoed back from the ESP8266.
4. Open the ESP8266's own Serial Monitor as a cross-check: each accepted reading should also print `{"code":"ack"}` there.
5. Open the ESP8266's web page or `GET /value`: the value should track the P3T1755 reading.

### Extend the Web App: Chart the Temperature Over Time

`data/index.html` currently only shows the latest value, re-fetched from `/value` every 500 ms and dropped once displayed. Use the same external-editor-plus-AI-agent workflow from [Use an External Editor with an AI Coding Agent](#4-use-an-external-editor-with-an-ai-coding-agent) to turn that polling loop into a chart of the reading over time.

A few constraints worth giving the agent up front, since `index.html` is served from flash (`data/index.html` -> `index_html.h`, embedded with `server.send_P`) on a board that may not have outbound internet access:

- keep the existing `/value` polling loop and JSON shape (`{"value": ...}`) — do not add a second endpoint or change the ESP8266 sketch unless the chart genuinely needs history kept on the device;
- do not pull in a charting library from a CDN; either draw the chart with plain `<canvas>` client-side, or vendor a small charting library's source directly into the page so the demo still works with no internet access;
- keep a bounded history (for example, the last 60 readings) so the page does not grow memory unbounded if left open for a long time.

The straightforward implementation accumulates each polled `{value, timestamp}` pair client-side in a JS array in `data/index.html`, and redraws a simple line/sparkline on a `<canvas>` each time a new point arrives — no ESP8266-side change needed at all, since the chart is just a different way of visualizing the same `/value` polling the page already does.

## Bring-Up Checklist

| Symptom | Likely cause | Fix |
|---|---|---|
| Board missing from Boards Manager | ESP8266 index URL not added | add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` in Preferences |
| Upload fails / times out | wrong port or board still in a boot-strap state | select the correct COM port; hold/release `FLASH` if the module needs manual boot mode |
| Serial Monitor shows garbage | wrong baud rate | set Serial Monitor to `115200` |
| Page never loads in the browser | board not connected to Wi-Fi, or wrong IP | check the serial status line for `wifiReady` and the current IP |
| `/value` never changes, no `{"code":"ack"}` printed | UART not wired, wired straight instead of crossed, or NXP side sends malformed JSON | check `TX2`/`RX2`/`GND` wiring; confirm the NXP UART line matches `{"code":"data","value":...}` exactly |
| Data reaches the ESP8266, but nothing shows up on the FRDM-MCXA153 console | the data line and the ESP8266's ack are only ever written to `LPUART2`, never mirrored/bridged to `LPUART0` | see [Iterating With the Feedback-To-Change-Spec Agent](#iterating-with-the-feedback-to-change-spec-agent); confirm the console still shows nothing after that fix — then check `TX2`/`RX2` wiring is crossed, not swapped with another header |

## Deliverable

Submit a project where:

1. the Arduino IDE builds and uploads the sketch to a NodeMCU 1.0 (ESP-12E) board without missing-library errors;
2. the Serial Monitor shows the board's Wi-Fi status and IP address;
3. the board serves the web page and the `/value` JSON endpoint over HTTP;
4. the FRDM-MCXA153 sensor node sends `{"code":"data","value":...}` readings over UART and the ESP8266 replies with `{"code":"ack"}`;
5. both the data line and the ack are visible on the FRDM-MCXA153's own console (not just on the ESP8266's Serial Monitor);
6. the web page also shows a chart of the temperature reading over time (see [Extend the Web App: Chart the Temperature Over Time](#extend-the-web-app-chart-the-temperature-over-time)), not just the current numeric value;
7. a short note describing one setup issue and how you solved it.
