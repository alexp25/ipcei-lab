---
title: "L6 - LPSPI - ILI9341 TFT LCD - Flappy Bird"
description: "SPI master, TFT display, and a simple embedded game"
nav_order: 7
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_shield_tft_lcd/main
---

# L6 - LPSPI - ILI9341 TFT LCD

**From a static SPI display demo to an interactive LCD game**

---

| | |
|---|---|
| **Session** | SPI |
| **Peripheral** | `LPSPI0` |
| **Hardware** | FRDM-MCXA153, ILI9341 SPI TFT module, jumper wires or compatible shield |

## Context and Motivation

SPI is widely used for displays, flash memories, ADC/DAC converters, IMUs, and LED drivers. In this lab, `LPSPI0` drives an ILI9341 TFT display.

The ILI9341 interface clearly separates:

- SPI bus: `SCK`, `MOSI`, optional `MISO`;
- device select: `CS`;
- command/data selection: `DC`;
- reset and backlight: `RST`, `BL`.

The final task transforms a static display demo into a simple Flappy-Bird-style game. The LCD is driven through `LPSPI0`, and `SW2` controls the bird.

## Objectives

By the end of the lab you should be able to:

1. Configure `LPSPI0` as SPI master.
2. Explain `CS`, `DC`, `RST`, and `BL` for ILI9341.
3. Implement command/data writes, address windows, pixels, rectangles, and lines.
4. Display a static demo with colors and text.
5. Build an interactive game loop controlled by `SW2`.
6. Debug common LCD wiring and SPI mode issues.

## Workflow Overview

The project setup has two parts:

1. Generated configuration owned by MCUXpresso Config Tools: the `LPSPI0` peripheral component, the peripheral clock, and SPI pin routing.
2. Application code owned by you and the coding agent: ILI9341 commands, GPIO control lines, drawing primitives, and demo behavior.

Do not manually edit `.mex` or generated board files. Make peripheral, clock, and pin changes in Config Tools, then use **Update Code**.

The recommended approach from the configuration guide is:

1. Configure `LPSPI0` and add its generated configuration component in the **Peripherals** tool.
2. Configure the `LPSPI0` clock in the **Clocks** tool.
3. Configure the SPI interface pins in the **Pins** tool.
4. Update generated code.
5. Use the AI-agent workflow to inspect the generated configuration, produce a specification, generate only application-owned code, and iterate from real build/run feedback.

## 1. Configure LPSPI0 In Peripherals

Open the project `.mex` file with MCUXpresso Config Tools. In the **Peripherals** tool, enable `LPSPI0` as a master peripheral and add the configuration component for generated initialization.

![Configure LPSPI0 as a master peripheral](../img/lab_lpspi_tft_lcd/01-configure-lpspi0-peripheral.png)

Use a conservative display bring-up configuration:

| Setting | Value |
|---|---|
| Peripheral | `LPSPI0` |
| Mode | Master |
| Frame size | 8 bits |
| Bit order | MSB first |
| Clock polarity | `0` |
| Clock phase | `0` |
| PCS handling | Use the generated LPSPI configuration only if your wiring maps LCD `CS` to an LPSPI PCS signal; otherwise control LCD `CS` as GPIO |
| Baud rate | `10000000` Hz for the final lab setup |

The ILI9341 command interface uses 8-bit transfers. Pixel data is RGB565, so each pixel is sent as two 8-bit bytes: high byte first, then low byte.

The MCUXpresso Config Tools default may create the LPSPI component with a conservative `500 kHz` baud rate. That is useful for first bring-up, but it makes full-screen LCD refresh very slow. For the final lab configuration, set the `LPSPI0` baud rate to `10 MHz`. The [ILI9341 datasheet](https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf) lists a minimum 4-wire serial write clock cycle of `100 ns`, which corresponds to a maximum write clock of:

```text
1 / 100 ns = 10 MHz
```

Use this `10 MHz` setting only after the display works at a slower clock. It is appropriate for the short breadboard jumper wires used in the lab when the wiring is tidy, the ground connection is solid, and the SPI wires are kept short. If the display shows random pixels, intermittent blanking, or unstable colors, reduce the baud rate and inspect `SCK`, `MOSI`, `CS`, `DC`, and ground before changing the driver code.

## 2. Configure The LPSPI0 Clock

Open the **Clocks** tool and enable the clock source used by `LPSPI0`.

![Configure the LPSPI0 clock](../img/lab_lpspi_tft_lcd/02-configure-lpspi0-clock.png)

Confirm that the generated clock output is active and that its frequency matches the generated `LPSPI0` configuration. Use generated clock macros or generated peripheral initialization; do not hard-code a conflicting SPI clock in application code.

![Check LPSPI0 clock details](../img/lab_lpspi_tft_lcd/03-lpspi0-clock-details.png)

![Confirm LPSPI0 clock output](../img/lab_lpspi_tft_lcd/04-lpspi0-clock-confirm.png)

If the display remains blank even though GPIO control lines look correct, reduce the SPI baud rate first. A slower clock is easier to validate with jumper wires and long breadboard connections. Once the wiring is known-good, return to `10 MHz` to improve game refresh rate.

## 3. Configure The SPI Pins

Open the **Pins** tool and route the SPI signals used by your LCD wiring:

![Configure the SPI interface pins](../img/lab_lpspi_tft_lcd/05-configure-spi-pins.png)

| LCD signal | Required? | Pin tool configuration |
|---|---|---|
| `SCK` | yes | route to `LPSPI0_SCK` |
| `MOSI` | yes | route to `LPSPI0_SOUT` / MOSI |
| `MISO` | optional | route to `LPSPI0_SIN` only if the application reads from the LCD |
| `CS` | yes | use `LPSPI0_PCS` only if configured as hardware chip select; otherwise configure as GPIO |
| `DC` | yes | GPIO output |
| `RST` | yes | GPIO output |
| `BL` | module dependent | GPIO output or fixed power rail |

Keep `DC`, `RST`, and `BL` as GPIO lines controlled by the application or board support code. Do not configure them as SPI pins.

![Check SPI pin details](../img/lab_lpspi_tft_lcd/06-spi-pin-details.png)

## 4. Update Generated Code

After the peripheral, clock, and pin settings are correct, use **Update Code**.

Review the proposed changes before accepting them. Generated files should remain in the active project folder and should match the files compiled by CMake.

The generated application startup should keep this shape:

```c
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockFRO12M();
    BOARD_InitBootPeripherals();
}
```

The root application file should call `BOARD_InitHardware()` once at startup. Application code should then initialize LCD GPIO control lines, initialize the ILI9341 controller, and enter the demo update loop.

## LCD Module Used In This Lab

This lab uses the 2.4 inch BITMI/OEM ILI9341 TFT module with touch and microSD slot:

[Ecran LCD ILI9341, cu touch si slot pentru Card SD, 2.4"](https://www.bitmi.ro/electronica/ecran-lcd-ili9341-cu-touch-si-slot-pentru-card-sd-2-4-10797-bitmi-ro.html)

<img src="../img/lab_lpspi_tft_lcd/lcd-module-front.png" alt="ILI9341 LCD module front" width="360">

<img src="../img/lab_lpspi_tft_lcd/lcd-module-back.png" alt="ILI9341 LCD module back with pin labels" width="360">

The product page identifies the module as a 2.4 inch ILI9341 LCD with touch and an SD card slot. The rear silkscreen on the module exposes a 14-pin header for the display and touch interface, plus separate SD card pads.

Main header labels visible on the module:

| Pin label | Meaning in this lab |
|---|---|
| `VCC` | module power input; verify whether your exact board is wired for 3.3 V or 5 V before connecting |
| `GND` | common ground |
| `CS` | LCD chip select |
| `RESET` | LCD reset |
| `DC` | LCD command/data select |
| `SDI(MOSI)` | SPI data from MCU to LCD |
| `SCK` | SPI clock |
| `LED` | backlight input or backlight enable, depending on module assembly |
| `SDO(MISO)` | SPI data from LCD to MCU; optional for this lab |
| `T_CLK` | touch controller SPI clock |
| `T_CS` | touch controller chip select |
| `T_DIN` | touch controller SPI input |
| `T_DO` | touch controller SPI output |
| `T_IRQ` | touch interrupt output |

The SD card pads are labeled separately as `SD_CS`, `SD_MOSI`, `SD_MISO`, and `SD_SCK`. They are not used in this lesson. Do not connect the touch or SD card signals unless the application explicitly configures and uses them.

## ILI9341 Connections


![Circuit Schematic](../img/lab_lpspi_tft_lcd/schematic.png)


| LCD signal | Role | FRDM-MCXA153 |
|---|---|---|
| `SCK` | SPI clock | configured LPSPI0 SCK pin |
| `MOSI` | MCU -> LCD data | configured LPSPI0 MOSI pin |
| `MISO` | optional LCD -> MCU data | optional |
| `CS` | chip select, active-low | GPIO in `board_ili9341.c` |
| `DC` | command/data select | GPIO in `board_ili9341.c` |
| `RST` | controller reset | GPIO in `board_ili9341.c` |
| `BL` | backlight | GPIO or power rail |
| `VCC/GND` | power | module dependent |

Always verify module voltage requirements. Some breakouts include regulators and level shifters; others require 3.3 V signals.

## SPI Concepts

For ILI9341, the typical transaction is:

1. `CS = 0` selects the LCD.
2. `DC = 0` before a command byte.
3. `DC = 1` before command payload or pixel data.
4. `CS = 1` ends the transaction.

Common ILI9341 commands:

| Command | Meaning |
|---|---|
| `0x01` | software reset |
| `0x11` | sleep out |
| `0x29` | display on |
| `0x2A` | column address set |
| `0x2B` | page address set |
| `0x2C` | memory write |

Most ILI9341 modules use SPI Mode 0:

| Parameter | Value |
|---|---|
| `CPOL` | `0` |
| `CPHA` | `0` |

If the SPI mode is wrong, the display can remain blank or show random pixels even when wiring is correct.

## RGB565 Pixels

The lab uses RGB565, 16 bits per pixel:

| Component | Bits | Range |
|---|---|---|
| Red | 5 | `0..31` |
| Green | 6 | `0..63` |
| Blue | 5 | `0..31` |

Examples:

```c
#define ILI9341_COLOR_BLACK   0x0000U
#define ILI9341_COLOR_RED     0xF800U
#define ILI9341_COLOR_GREEN   0x07E0U
#define ILI9341_COLOR_BLUE    0x001FU
#define ILI9341_COLOR_WHITE   0xFFFFU
```

Efficient drawing uses address windows: set a rectangle with `0x2A` and `0x2B`, then stream pixels with `0x2C`.

## Code Structure

Relevant files:

| File | Role |
|---|---|
| `board_ili9341.c/.h` | LCD pin mapping and GPIO setup for `CS`, `DC`, `RST`, `BL` |
| `ili9341.c/.h` | low-level ILI9341 driver |
| `ili9341_demo.c/.h` or `game.c/.h` | demo, drawing, and game logic |
| `main.c` | application initialization and main loop |

Recommended API:

```c
status_t ILI9341_GameInit(void);
void ILI9341_GameUpdate(bool flap_pressed);
```

`flap_pressed` should be true when `SW2` is pressed. Keep button reading outside the low-level ILI9341 driver so the display code remains reusable.

## Lab Task

Starting from a static ILI9341 demo, build a simple game:

1. Keep LCD initialization and drawing functions.
2. Replace the static screen with a frame-based game loop.
3. Draw a bird, obstacles, ground, and score using simple rectangles, lines, or bitmap-like primitives.
4. Use gravity to move the bird downward between button presses.
5. Use `SW2` to make the bird fly higher.
6. Detect collisions with obstacles and the ground.
7. Keep `main.c` small: initialize hardware, read `SW2`, and call the game update function.

Example integration:

```c
status_t status = ILI9341_GameInit();

while (1)
{
    bool flap = BOARD_SW2IsPressed();
    ILI9341_GameUpdate(flap);
}
```

## Debug Checklist

| Symptom | Likely cause | Check |
|---|---|---|
| Black screen | backlight off or no power | `BL`, `VCC`, `GND` |
| White screen | controller not initialized or reset wrong | reset sequence, `RST` pin |
| Random pixels | wrong SPI mode or bad wires | CPOL/CPHA, `SCK`, `MOSI`, common GND |
| Wrong colors | RGB/BGR or RGB565 issue | `MADCTL`, color conversion |
| Rotated image | orientation config | `ili9341_set_rotation()` |
| Text works, animation does not | update not called | `ILI9341_GameUpdate()` in loop |
| Animation too slow | too much redraw | reduce redraw area or SPI overhead |

Start debugging with simple tests:

1. enable backlight;
2. fill screen red;
3. fill screen green;
4. fill screen blue;
5. draw a diagonal line;
6. start animation only after these pass.

## AI Agent Workflow

This lesson can be implemented with the coding-agent workflow from [LP2 - From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent).

The agent workflow mirrors the Config Tools process above: inspect generated configuration first, generate only application-owned code next, then convert build or board feedback into a focused change specification.

### Agent 1: Planning / Configuration Inspection

```text
Use .agents/mcux-config-to-main-spec.prompt.md.

APPLICATION_TASK: implement a simple game similar to Flappy Bird using the LCD connected to LPSPI0, and the SW2 button to make the bird fly higher.
Use the current working directory as PROJECT_ROOT.

Return the specification only. Do not edit files.
```

Review the specification before continuing. Continue only if it confirms:

- active root C file;
- `BOARD_InitHardware()` startup path;
- generated `LPSPI0` peripheral configuration and clock availability;
- SPI pin routing for the LCD bus;
- GPIO availability for `CS`, `DC`, `RST`, `BL`, and `SW2`;
- SDK driver availability for `fsl_lpspi` and `fsl_gpio`;
- existing behavior that must be preserved.

If the specification reports `BLOCKED`, fix the Config Tools setup first. If it reports `PARTIAL`, generate only the behavior that is explicitly confirmed as available.

### Agent 2: Coding

```text
Use .agents/mcux-main-codegen-from-spec.prompt.md with the specification above.

Use the current working directory as PROJECT_ROOT.
Apply code changes only to application-owned files named by the specification.
Do not edit .mex or generated board files.
```

The coding agent must follow these rules:

- edit only the active root application C file and/or project-local files under `app/` if the specification names them as application-owned;
- preserve existing behavior unless the specification explicitly requires a change;
- keep `BOARD_InitHardware()` as the board initialization entry point;
- use generated macros and SDK driver calls confirmed by the specification;
- do not add unconfirmed peripherals, pins, clocks, IRQs, channels, or direct register writes;
- do not edit `.mex`, `pin_mux.c`, `clock_config.c`, `peripherals.c`, files under `frdmmcxa153/`, or other generated configuration files;
- return the complete updated contents of each edited application-owned file, a short explanation, preserved behavior, relevant warnings, and a build command if discoverable.

Before copying the generated code into the project, check that it:

- compiles as C;
- has exactly one `main` function across the project;
- keeps generated initialization in the startup path;
- uses `LPSPI0` for LCD SPI transfers;
- keeps `DC`, `RST`, `BL`, and software `CS` control as GPIO unless the specification confirms hardware PCS;
- reads `SW2` without blocking the display update loop.

Updated agent code from `src/lab_new_project_configured/configured4/.agents/mcux-main-codegen-from-spec.prompt.md`:

````markdown
# Prompt: Generate Application Code From MCUXpresso Specification

You are an embedded C code generator for an MCUXpresso SDK project.

Your job is to implement code only in application-owned source files named by the specification, using the specification produced by the previous configuration-analysis prompt. Application-owned files include the active root application C file, for example `main.c` or `led_blinky.c`, and project-local files under `app/`, for example `app/*.c` and `app/*.h`. You must preserve generated files and existing application behavior unless the specification explicitly says otherwise.

## Inputs

Specification from the previous prompt:

```markdown
{{SPECIFICATION}}
```

Project root:

```text
{{PROJECT_ROOT}}
```

## Hard Rules

- Edit only application-owned files named in the specification: the active root C file and/or files under `app/`.
- Do not edit files outside the active root C file or `app/` unless the specification explicitly identifies them as application-owned and non-generated.
- Do not edit `.mex` files.
- Do not edit generated files under the board/config folder, for example `frdmmcxa153/`.
- Preserve existing functionality by default. Do not remove existing features, handlers, initialization calls, helper functions, includes, state machines, debug output, or loop behavior unless the specification explicitly requires that removal.
- Add new behavior incrementally and integrate it with existing behavior.
- If the specification is ambiguous about whether existing code should remain, keep it and adapt around it.
- If a new feature conflicts with existing behavior, implement the conflict resolution described by the specification. If no resolution is specified, stop and return a warning instead of deleting existing behavior.
- Do not add code for peripherals, pins, channels, IRQs, clocks, or macros that are not explicitly confirmed by the specification.
- If the specification status is `BLOCKED`, do not generate functional application code. Return the blocking warnings and stop.
- If the specification status is `PARTIAL`, implement only the capabilities explicitly marked as available and preserve warnings as comments only when they are directly relevant to the app code.
- Keep `BOARD_InitHardware()` as the board initialization entry point unless the specification explicitly says otherwise.
- Keep the code simple, idiomatic MCUXpresso SDK C.
- Prefer existing generated macros and config structs over hard-coded peripheral addresses, pin numbers, channels, frequencies, or clock values.

## Code Generation Process

1. Open each application-owned file named in the specification.
2. Read its current contents.
3. Identify existing application behavior that must be preserved.
4. Replace or modify only the application-owned code needed for the requested behavior.
5. Preserve license/SPDX header if present.
6. Preserve required includes such as `board.h` and `app.h`.
7. Add generated headers such as `pin_mux.h` or `peripherals.h` only when the specification requires their macros or config structs directly.
8. Use SDK driver calls named in the specification.
9. Ensure all identifiers used in the code are either defined in edited application files, standard C headers, included MCUXpresso SDK headers, or confirmed generated project headers.
10. Do not leave placeholder functions, TODOs, pseudocode, markdown, or unexplained stubs in edited files.

## Output Requirements

Return:

1. The complete updated contents of each edited application-owned file, each in its own code block labeled with the file path.
2. A short explanation of what changed.
3. A short list of existing behaviors that were preserved.
4. Any warnings inherited from the specification that still matter after generation.
5. Suggested build command if one is discoverable from project files; otherwise say that the project should be built with its existing MCUXpresso SDK/CMake workflow.

## Acceptance Checks

The generated application code must:

- Compile as C, not C++.
- Contain exactly one `main` function across the project.
- Keep generated initialization in the startup path.
- Use only configuration confirmed by the specification.
- Preserve existing behavior unless the specification explicitly requires a change.
- Avoid modifying generated configuration.
- Avoid direct register writes unless the specification explicitly requires them and the relevant register definitions are available.
- Avoid busy waiting with magic delays when an existing timing source or configured peripheral is specified.
````

### Agent 3: Review / Debug Feedback

Use this when the program builds but the LCD behavior is wrong. First convert the observation into a change specification, then run Agent 2 again if the change specification is accurate.

```text
Use .agents/mcux-feedback-to-change-spec.prompt.md.

FEEDBACK: <your observations>.

INTENDED_BEHAVIOR: the LCD connected to LPSPI0 should initialize correctly, run a simple Flappy-Bird-style game, and use SW2 to make the bird fly higher.

Use the current working directory as PROJECT_ROOT.
Do not edit files. Return the change specification only.
```

Example feedback:

```text
FEEDBACK: the display backlight turns on, but the screen stays white
```

Likely areas to inspect are the reset sequence, `DC` and `CS` polarity, SPI mode, and whether the generated `LPSPI0` initialization is actually called.

## AI Assistant Reference Prompts

The prompts below are useful for smaller manual reviews or targeted changes. For the full lab implementation, prefer the three-agent workflow above.

### Prompt: ILI9341 Driver over LPSPI

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK.
Display: ILI9341 TFT SPI, RGB565.
SPI: LPSPI0 master, mode 0, MSB first.
GPIO lines: CS, DC, RST, BL.
Implement or review: init, write_command, write_data, set_address_window, fill_rect, fill_screen, draw_line.
Use MCUXpresso SDK APIs fsl_lpspi and fsl_gpio. Do not use Arduino APIs.
```

### Prompt: Static Demo to Game

```text
I have a working ILI9341 driver on FRDM-MCXA153 with fill_screen, fill_rect, draw_line, and write_string.
Transform the static demo into a simple Flappy-Bird-style game:
- bird affected by gravity;
- SW2 press makes the bird fly higher;
- scrolling obstacles;
- score display;
- collision and restart behavior;
- API: status_t ILI9341_GameInit(void); void ILI9341_GameUpdate(bool flap_pressed).
Embedded C only: no malloc, no float, no external graphics libraries.
```

## Common Pitfalls

- Do not confuse `DC` with `CS`.
- ILI9341 uses RGB565 in this lab, not RGB888.
- Avoid redrawing the full screen for every frame.
- If the screen is black, check power, backlight, reset, `CS`, `DC`, pin mux, and SPI clock.
- If animation flickers, reduce redraw area or slow the update loop.
- Debounce or edge-detect `SW2` if one press causes too many flaps.
- Keep wires short and ensure common ground.

## Deliverable

Submit a project that:

1. initializes the ILI9341 display;
2. draws a basic static test;
3. implements a simple Flappy-Bird-style game;
4. uses `SW2` to control the bird;
5. exposes `ILI9341_GameUpdate(flap_pressed)` for the main loop.

**Task: upload the Gen AI chat to the evaluation platform**

---

[<- L5: LPADC - Analog Voltage Measurement](../l5-lpadc) - [L7: LPI2C - P3T1755 Temperature Sensor ->](../l7-lpi2c-p3t1755)
