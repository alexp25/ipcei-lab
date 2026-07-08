---
title: "LP4 - ESP8266 IoT App Setup"
description: "Flash a ready-made ESP8266 + FRDM-MCXA153 IoT demo, find the board's IP over serial, and test the web page and MQTT chat live against a projected broker dashboard"
nav_order: 5
parent: Additional Demos
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src_arduino/simple_server_esp8266
---

# LP4 - NXP & ESP8266 IoT App Setup

**Upload the ESP8266 + FRDM-MCXA153 IoT demo, then bring it up and test it: find the board on the network from serial output alone, drive the web page, and chat over a shared MQTT broker that's projected live during the session.**

---

| | |
|---|---|
| **Session** | IoT / Wi-Fi bring-up and test session, separate from the FRDM-MCXA153 track |
| **Board** | ESP8266 NodeMCU 1.0 (ESP-12E Module) |
| **Tooling** | Arduino IDE 1.8.x, ESP8266 Boards package |
| **Console** | Serial Monitor, 115200 baud |
| **Local project (ESP8266)** | `src_arduino/simple_server_esp8266` — `https://github.com/alexp25/ipcei-lab/tree/main/src_arduino/simple_server_esp8266` (already implemented — upload as-is) |
| **Sensor node project (NXP)** | `src/lab_new_project_configured/configured_iot` — `https://github.com/alexp25/ipcei-lab/tree/main/src/lab_new_project_configured/configured_iot` (FRDM-MCXA153 + P3T1755, already implemented — flash as-is) |
| **MQTT broker** | `isilab.cloud.shiftr.io:1883`, topic `/general` — dashboard projected live during the session |
| **Main deliverable** | Both boards flashed and wired, the web page reachable and showing live temperature + chat, verified against the projected broker dashboard |

## Context

The code for this lab is done: `simple_server_esp8266` (ESP8266 Wi-Fi + web page + MQTT chat relay) and `configured_iot` (FRDM-MCXA153 + P3T1755 sensor, sending readings and chat over UART) are both ready to flash. Your job in this session is not to write code — it's to bring the system up and prove, hands-on, that it actually works end to end:

1. flash both boards;
2. find the ESP8266 on the Wi-Fi network using only its serial output — no router admin page, no network scanner;
3. open the web page in a browser and watch the temperature update live;
4. figure out how to use the chat panel, without a click-by-click recipe;
5. cross-check your own chat messages against a shared broker dashboard that the instructor projects for the whole room, so everyone can see every board's traffic on `/general` in real time.

If something doesn't come up, the [Bring-Up Checklist](#bring-up-checklist) below is your first stop. If you're curious how any of this was actually implemented (the UART bridge, the MQTT relay, the AI-agent workflow used to build it), that's all in the [Appendix](#appendix-how-this-was-built-optional-deep-dive) — optional reading, not required for this session.

## Objectives

By the end of this session you should be able to:

1. install ESP8266 board support in the Arduino IDE and upload the provided sketch;
2. wire the FRDM-MCXA153 and the ESP8266 together over UART;
3. read the ESP8266's serial status output to find its IP address, with no other way of discovering it;
4. reach the ESP8266's web page from a browser and confirm the temperature value tracks the P3T1755 sensor;
5. use the chat panel on the web page, and verify a message you send is visible both in your own browser and on the shared broker dashboard projected in the room;
6. diagnose a non-working bring-up (no IP, no page, no chat) using the checklist instead of guessing.

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

## Physical Wiring: FRDM-MCXA153 to ESP8266

The FRDM-MCXA153 side uses two separate UART instances so the ESP8266 link and the local debug console don't share a wire: `LPUART0` (header pins `RX0`/`TX0`) stays the console, and a second instance, `LPUART2`, is dedicated to the ESP8266 link. Connect the two boards' UARTs directly, crossed, with a common ground:

| FRDM-MCXA153 (`LPUART2`, pins `62`/`63`) | ESP8266 NodeMCU |
|---|---|
| `TX2` (`P1_5`, pin `63`) | `RXD` (`Serial` RX) |
| `RX2` (`P1_4`, pin `62`) | `TXD` (`Serial` TX) |
| `GND` | `GND` |

Both sides are already configured for `115200 8N1`. `LPUART0` keeps its own separate `115200 8N1` config and its `RX0`/`TX0` header pins free for a Serial Monitor / terminal on the PC — that's the console you'll actually use in the next steps.

![Wiring diagram: FRDM-MCXA153 to ESP8266 NodeMCU, TX/RX crossed with a shared GND and 3V3 reference](../img/labp4_iot_app/circuit_image.png)

The diagram shows the same connection made through the board's Arduino-compatible headers rather than the bare `RX0`/`TX0`/`TX2`/`RX2` pads: TX crossed to RX, RX crossed to TX, plus `GND` and `3V3` tied between the two boards for a shared reference. Tying `GND` together is required for the UART signals to read correctly; tying `3V3` together is optional and only matters if you want a shared logic reference beyond `GND` — each board is still powered independently from its own USB cable.

## Bring-Up and Testing

### 1. Flash Both Boards

Upload `configured_iot` to the FRDM-MCXA153 and `simple_server_esp8266` to the ESP8266 — no code changes needed, just build and upload each project as-is.

- NXP project (FRDM-MCXA153): `https://github.com/alexp25/ipcei-lab/tree/main/src/lab_new_project_configured/configured_iot`
- ESP8266 sketch: `https://github.com/alexp25/ipcei-lab/tree/main/src_arduino/simple_server_esp8266`

### 2. Find the ESP8266's IP Address via UART

Do this before you try a browser at all — without it you have no address to browse to, and there is no other supported way to find the board on the network.

Open a Serial Monitor / terminal at `115200` baud, either directly on the ESP8266's own USB port, or on the FRDM-MCXA153's `LPUART0` console (which mirrors the ESP8266 link). Watch for a line like:

```json
{"code":"status","data":"192.168.1.42","wifiReady":true}
```

`data` is the ESP8266's current IP address; `wifiReady` tells you whether it actually joined the Wi-Fi network yet. This line repeats about once a second, so if `wifiReady` is `false`, just wait — or see the checklist if it never flips to `true`.

### 3. Open the Web Page in a Browser

Point a browser on the same network at `http://<the IP from step 2>/`. You should see the current temperature reading, updating roughly twice a second, tracking the FRDM-MCXA153's P3T1755 sensor.

### 4. Use the Chat

The page also has a chat panel. Figure out how to send a message and see what comes back — there's no trick beyond what's visibly on the page. A couple of things worth noticing once you've tried it:

- a message you send does not appear in your own panel immediately;
- everyone's messages, from any browser or device connected to the same topic, end up in the same panel.

Think about why, and check your explanation against the [protocol reference](#chat-and-uart-protocol-reference) below if you want to confirm it.

### 5. Watch the Shared MQTT Broker Dashboard

The instructor will project the `isilab.cloud.shiftr.io` broker's own live dashboard for the room. It shows every client currently connected and every message flowing across `/general` in real time, independent of anyone's browser page.

Send a chat message from your own board's web page, then find it on the projected dashboard. If you can identify your own messages there — and see other people's boards publishing too — you've verified the whole chain: your board, the broker, and everyone else's boards are all really talking to the same topic, not just to themselves.

## Chat and UART Protocol Reference

Useful for troubleshooting, not required reading to use the chat panel itself.

The ESP8266 reads newline-terminated JSON lines on `Serial` (the same wire as the FRDM-MCXA153 link) and acknowledges any line that parses as JSON with `{"code":"ack","echo":"<the line that was received>"}`, whatever its `"code"`. Two `"code"` values have an actual effect:

- `"code":"data"` with a numeric `"value"` updates the displayed temperature:

  ```json
  {"code":"data","value":21.50}
  ```

- `"code":"chat"` with a string `"value"` is published to the MQTT `/general` topic, and, in the other direction, anything the ESP8266 receives on `/general` (from the web page, another MQTT client, or its own relayed message) is written back down the same UART the same way:

  ```json
  {"code":"chat","value":"hello from the NXP board"}
  ```

The web page's chat panel works the same way from the browser side: sending a message publishes it to `/general`, and the panel only shows a message once it comes back through the MQTT subscription — which is why your own message doesn't appear instantly, and why it looks identical whether it came from your browser, someone else's browser, or a board's UART.

## Bring-Up Checklist

| Symptom | Likely cause | Fix |
|---|---|---|
| Board missing from Boards Manager | ESP8266 index URL not added | add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` in Preferences |
| Upload fails / times out | wrong port or board still in a boot-strap state | select the correct COM port; hold/release `FLASH` if the module needs manual boot mode |
| Serial Monitor shows garbage | wrong baud rate | set Serial Monitor to `115200` |
| Status line never shows `wifiReady:true` | wrong Wi-Fi credentials baked into the sketch, or out of range | double-check the SSID/password in the sketch match the network you're actually on |
| Page never loads in the browser | using the wrong IP, board not actually on the same network/subnet, or `wifiReady` never became `true` | re-read the status line; confirm your PC and the board are on the same Wi-Fi network |
| Temperature never changes on the web page | UART not wired, wired straight instead of crossed, or no `{"code":"ack",...}` appears on the console | check `TX2`/`RX2`/`GND` wiring is crossed, not swapped with another header; a missing ack means the FRDM-MCXA153 isn't sending valid JSON at all |
| Nothing from the FRDM-MCXA153 shows up on its own `LPUART0` console | expected if you're only watching the ESP8266's own Serial Monitor — the two consoles show the same exchange from either end; check you're looking at the right port |
| Chat message never appears anywhere, even after sending | MQTT never connected — board not on Wi-Fi yet, or broker unreachable | wait for `wifiReady:true` first; the board retries the MQTT connection every 5 s once Wi-Fi is up |
| Your message shows in your own panel but never on the projected broker dashboard (or vice versa) | you and the dashboard may be looking at different topics/instances | confirm the topic is exactly `/general` and the broker is `isilab.cloud.shiftr.io` |
| A message sent from the web page never shows up in its own chat panel | the publish itself likely failed | check the browser's network tab: the `/chat` POST should return `200`, not `503` |

## Deliverable

Submit a project where:

1. both the FRDM-MCXA153 and the ESP8266 are flashed and wired together;
2. you can show the ESP8266's IP address coming from the serial status line alone (not a router page or network scanner);
3. the web page loads at that IP and the displayed temperature tracks the P3T1755 sensor;
4. you sent a chat message from the web page and can point to it on the shared broker dashboard projected in the room;
5. a short note describing one bring-up issue you hit and how you diagnosed it (which checklist row it matched, or what you found instead).

## Appendix: How This Was Built (Optional Deep Dive)

Everything below documents how `simple_server_esp8266` and `configured_iot` were actually implemented. None of it is required for this session's deliverable — it's here for anyone who wants to understand or extend the system after the bring-up/test exercise above.

### Use an External Editor with an AI Coding Agent

The built-in Arduino editor has no AI assistant. Enable **File -> Preferences -> Use external editor**, then open the sketch folder in an editor such as VS Code. Arduino IDE keeps compiling and uploading; the external editor (with an AI coding agent) is only used to edit files.

![Arduino IDE Preferences - Use external editor enabled](../img/labp4_iot_app/3_external_editor_preferences.png)

With the sketch open in VS Code, use an AI coding agent to add or change features. For example, asking the agent to change the UART protocol to JSON:

![AI coding agent editing simple_server_esp8266.ino to add a JSON UART protocol](../img/labp4_iot_app/4_ai_agent_feature_edit.png)

Save the file in the external editor, then switch back to Arduino IDE to verify and upload — Arduino IDE reloads the sketch from disk automatically.

### MQTT Chat: Broker, Library, and Web UI Implementation

The sketch connects to a shared lab broker:

| Setting | Value |
|---|---|
| Host | `isilab.cloud.shiftr.io` |
| Port | `1883` (plain MQTT, no TLS) |
| Username / password | `isilab` / see `mqttUser`/`mqttPassword` in the sketch |
| Topic | `/general` |

These live as plain constants (`mqttHost`, `mqttPort`, `mqttUser`, `mqttPassword`, `mqttChatTopic`) near the top of `simple_server_esp8266.ino`.

MQTT itself needs the `PubSubClient` library (by Nick O'Leary), which is not part of the ESP8266 core. Two ways to get it:

1. **Library Manager**: **Sketch -> Include Library -> Manage Libraries**, search `PubSubClient`, install. This is the normal path and keeps the library outside the sketch folder.
2. **Vendor it next to the sketch**: download `PubSubClient.h` and `PubSubClient.cpp` (for example from [arduinolibraries.info](https://www.arduinolibraries.info/libraries/pub-sub-client)) and drop both files directly next to the `.ino`, the same way `ArduinoJson-v7.4.3.h` is already vendored in this project. Arduino's builder compiles any `.cpp` file it finds in the sketch folder and adds that folder to the include path, so `#include <PubSubClient.h>` resolves either way — no Library Manager access needed.

The web page polls `GET /chat` every 500 ms, same cadence as `/value`, for a JSON array of the last messages seen on `/general`, and `POST /chat` (the message as a raw text body) to send a new one. Sending does **not** add the message to the panel directly — a message only appears once it comes back through the MQTT subscription, so the chat panel and the MQTT topic are always showing the exact same thing, from any client. This is the mechanism behind the "why doesn't my own message show up instantly" observation from the bring-up exercise.

`handleMqttConnection()` mirrors the existing Wi-Fi reconnect logic: it is non-blocking, only runs once Wi-Fi is connected, retries every 5 seconds, and re-subscribes to `/general` on every successful `connect()`.

### Implement the NXP Side With the Spec-Then-Codegen Agents

`configured_iot/.agents/` has three reusable prompts described in [From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent): `mcux-config-to-main-spec.prompt.md`, `mcux-main-codegen-from-spec.prompt.md`, and `mcux-feedback-to-change-spec.prompt.md`. This workflow — instead of hand-writing the UART code — is how the NXP side of this demo was built, with the agent verifying the P3T1755/LPUART configuration before generating anything.

Before running the spec agent, a second LPUART instance was added in MCUXpresso Config Tools (`LPUART2`), routed to a spare pair of pins, `115200 8N1`, RX and TX both enabled, and the project regenerated. This keeps `LPUART0` free to stay the console instead of overloading one wire with both the human-readable trace and the machine-readable ESP8266 protocol.

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

If the specification comes back `BLOCKED` (for example, if `LPUART2` were not routed to external pins), the fix is to reconfigure in MCUXpresso Config Tools and regenerate before trying again — never let the agent invent pins or peripherals.

### Iterating With the Feedback-To-Change-Spec Agent

Once the two boards were talking, a natural follow-up observation was: *"it sends the temperature all right, it arrives on the ESP8266, but I don't see the data in the console (Serial Monitor)."* That is a runtime observation, not a new feature request, so instead of guessing at a fix, it was fed to `mcux-feedback-to-change-spec.prompt.md`:

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

The agent traced through the generated config and the application code to confirm the actual cause: the JSON line was only ever written to `LPUART2`, and `LPUART0` was a separate, otherwise-idle peripheral, so nothing about the ESP8266 exchange was ever going out on the wire the console was watching. The resulting change specification was small and targeted — mirror the outgoing JSON line onto `LPUART0`, and add a byte-for-byte bridge between `LPUART0` and `LPUART2` so the ESP8266's reply becomes visible too — and it fed straight back into `mcux-main-codegen-from-spec.prompt.md` (Step 2) to implement. This is the same three-prompt loop as [From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent): spec, codegen, then feedback-to-change-spec for anything the first pass didn't get right.

That bridge is why the [Bring-Up and Testing](#bring-up-and-testing) steps above can use either console interchangeably: `LPUART0` mirrors every outgoing `{"code":"data",...}` line and echoes back everything received on `LPUART2` (including the ESP8266's ack and any chat traffic), and it's a simple byte-for-byte relay with no framing — so avoid typing into the console while a line is in flight, since it can interleave with whatever is mid-transmission to the ESP8266.

### Extend the Web App: Chart the Temperature Over Time

`data/index.html` only shows the latest value by default, re-fetched from `/value` every 500 ms and dropped once displayed. The same external-editor-plus-AI-agent workflow from [Use an External Editor with an AI Coding Agent](#use-an-external-editor-with-an-ai-coding-agent) turned that polling loop into a chart of the reading over time.

Constraints given to the agent up front, since `index.html` is served from flash (`data/index.html` -> `index_html.h`, embedded with `server.send_P`) on a board that may not have outbound internet access:

- keep the existing `/value` polling loop and JSON shape (`{"value": ...}`) — do not add a second endpoint or change the ESP8266 sketch unless the chart genuinely needs history kept on the device;
- do not pull in a charting library from a CDN; either draw the chart with plain `<canvas>` client-side, or vendor a small charting library's source directly into the page so the demo still works with no internet access;
- keep a bounded history (for example, the last 60 readings) so the page does not grow memory unbounded if left open for a long time.

The implementation accumulates each polled `{value, timestamp}` pair client-side in a JS array in `data/index.html`, and redraws a simple line/sparkline on a `<canvas>` each time a new point arrives — no ESP8266-side change needed at all, since the chart is just a different way of visualizing the same `/value` polling the page already does.
