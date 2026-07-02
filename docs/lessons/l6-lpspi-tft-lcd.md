---
title: "L6 - LPSPI - ILI9341 TFT LCD"
description: "SPI master, TFT display, and a dynamic embedded visualizer"
nav_order: 7
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_shield_tft_lcd/main
---

# L6 - LPSPI - ILI9341 TFT LCD

**From a static SPI display demo to a dynamic visualizer**

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

The final task transforms a static display demo into a dynamic screen saver with a starfield and a 2D waveform.

## Objectives

By the end of the lab you should be able to:

1. Configure `LPSPI0` as SPI master.
2. Explain `CS`, `DC`, `RST`, and `BL` for ILI9341.
3. Implement command/data writes, address windows, pixels, rectangles, and lines.
4. Display a static demo with colors and text.
5. Build a dynamic visualizer controlled by an input value.
6. Debug common LCD wiring and SPI mode issues.

## ILI9341 Connections

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
| `ili9341_demo.c/.h` | demo and animation logic |
| `main.c` | application initialization and main loop |

Recommended API:

```c
status_t ILI9341_DemoInit(void);
void ILI9341_DemoUpdate(uint16_t input_value);
```

`input_value` is `0..4095`, matching a 12-bit ADC scale. It can start as simulated data and later come from LPADC or a sensor.

## Lab Task

Starting from a static ILI9341 demo, build a dynamic screen saver:

1. Keep LCD initialization and drawing functions.
2. Replace the static screen with animation.
3. Implement a full-screen starfield moving outward from the center.
4. Draw a 2D waveform in the center.
5. Link waveform amplitude to `input_value`.
6. Use `SW3` to change the color theme.
7. Keep `main.c` ready to replace simulated input with a real sensor value.

Example integration:

```c
status_t status = ILI9341_DemoInit();

while (1)
{
    uint16_t input = read_sensor_or_generate_demo_value();
    ILI9341_DemoUpdate(input);
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
| Text works, animation does not | update not called | `ILI9341_DemoUpdate()` in loop |
| Animation too slow | too much redraw | reduce redraw area or SPI overhead |

Start debugging with simple tests:

1. enable backlight;
2. fill screen red;
3. fill screen green;
4. fill screen blue;
5. draw a diagonal line;
6. start animation only after these pass.

## AI Assistant Prompts

### Prompt: ILI9341 Driver over LPSPI

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK.
Display: ILI9341 TFT SPI, RGB565.
SPI: LPSPI0 master, mode 0, MSB first.
GPIO lines: CS, DC, RST, BL.
Implement or review: init, write_command, write_data, set_address_window, fill_rect, fill_screen, draw_line.
Use MCUXpresso SDK APIs fsl_lpspi and fsl_gpio. Do not use Arduino APIs.
```

### Prompt: Static Demo to Visualizer

```text
I have a working ILI9341 driver on FRDM-MCXA153 with fill_screen, fill_rect, draw_line, and write_string.
Transform the static demo into a screen saver:
- full-screen starfield moving outward;
- 2D waveform in the center;
- amplitude controlled by input_value 0..4095;
- SW3 changes color theme;
- API: status_t ILI9341_DemoInit(void); void ILI9341_DemoUpdate(uint16_t input_value).
Embedded C only: no malloc, no float, no external graphics libraries.
```

## Common Pitfalls

- Do not confuse `DC` with `CS`.
- ILI9341 uses RGB565 in this lab, not RGB888.
- Avoid redrawing the full screen for every frame.
- If the screen is black, check power, backlight, reset, `CS`, `DC`, pin mux, and SPI clock.
- If animation flickers, reduce redraw area or slow the update loop.
- Keep wires short and ensure common ground.

## Deliverable

Submit a project that:

1. initializes the ILI9341 display;
2. draws a basic static test;
3. implements the starfield + waveform visualizer;
4. changes colors with SW3;
5. exposes `ILI9341_DemoUpdate(input_value)` for future sensor integration.

---

[<- L5: LPADC - Analog Voltage Measurement](../l5-lpadc) - [L7: LPI2C - P3T1755 Temperature Sensor ->](../l7-lpi2c-p3t1755)

