---
title: "Test L6 - LCD 1602 and Analog Keypad"
description: "Arduino LCD Keypad Shield integration in 4-bit mode with GPIO, LPADC, and MCUXpresso Config Tools"
nav_order: 7
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_lcd/main
---

# Test L6 - LCD 1602 and Analog Keypad

**Adding a new application to an existing MCUXpresso project and aligning it with Config Tools**

---

| | |
|---|---|
| **Peripherals** | `GPIO`, `ADC0/LPADC`, Arduino headers, `HD44780` |
| **Duration** | 2 h |
| **Hardware** | FRDM-MCXA153 + Arduino LCD Keypad Shield 1602 |
| **Project** | `src/lab_lcd/main` |

## Context

This draft lab integrates an Arduino LCD Keypad Shield 1602 into an existing MCUXpresso SDK project.

The shield contains:

- a 16x2 HD44780-compatible LCD;
- a 4-bit parallel data interface;
- five buttons connected through a resistor ladder to Arduino `A0`;
- optional backlight control.

The main learning goal is not only making the LCD work. The focus is the full integration process: inspect the existing project, write reusable drivers, configure pins in `.mex`, regenerate code, align CMake with generated files, and verify the final build.

## Objectives

By the end of the lab you should be able to:

1. Map Arduino shield pins to MCXA153 pins.
2. Explain HD44780 4-bit initialization.
3. Read an analog keypad with LPADC.
4. Scale 10-bit Arduino thresholds to a 12-bit ADC.
5. Use stable-sample filtering.
6. Keep reusable code separate from board-specific adapter code.
7. Add pins in MCUXpresso Config Tools.
8. Diagnose generated-file path mismatches.
9. Confirm that CMake builds the generated files you expect.

## Hardware Map

| Shield signal | Arduino pin | MCXA153 pin | Role |
|---|---|---|---|
| LCD `RS` | `D8` | `P2_7` | command/data select |
| LCD `EN` | `D9` | `P3_31` | enable pulse |
| LCD `D4` | `D4` | `P2_5` | data bit 0 |
| LCD `D5` | `D5` | `P3_12` | data bit 1 |
| LCD `D6` | `D6` | `P3_13` | data bit 2 |
| LCD `D7` | `D7` | `P3_1` | data bit 3 |
| Backlight | `D10` | `P3_11` | optional backlight control |
| Keypad | `A0` | `P1_10 / ADC0_A8` | resistor ladder |

`P3_12` and `P3_13` are shared with on-board LEDs, so they may flicker during LCD transfers.

## Electrical Compatibility

FRDM-MCXA153 GPIO and ADC pins are 3.3 V only. Do not connect a 5 V `A0` signal directly to the MCU ADC.

Check whether your shield is 3.3 V compatible. If not, use voltage division or level shifting. Any divider changes ADC thresholds, so measure raw values and adjust the table.

Backlight control is disabled by default until the shield schematic is verified:

```c
#define LCD_SHIELD_ENABLE_BACKLIGHT_CONTROL 0
```

## Software Architecture

```text
main.c
  |
  +-- hd44780.c/.h
  |     reusable LCD driver
  |
  +-- lcd_keypad.c/.h
  |     thresholds, scaling, debounce
  |
  +-- frdmmcxa153_lcd_shield.c/.h
        board adapter using SDK GPIO/LPADC and generated pin symbols
```

`hd44780.c` should not know board pins. `lcd_keypad.c` should not know ADC0 registers. Only the FRDM adapter connects reusable drivers to hardware.

## HD44780 Driver

The LCD works in 4-bit mode. Each byte is sent as two nibbles:

1. upper nibble on `D4-D7`;
2. pulse `EN`;
3. lower nibble on `D4-D7`;
4. pulse `EN` again.

`RS = 0` means command. `RS = 1` means character data.

The driver uses callbacks:

```c
typedef struct
{
    void *context;
    hd44780_control_write_t write_rs;
    hd44780_control_write_t write_enable;
    hd44780_data_write_t write_data;
    hd44780_delay_t delay_us;
} hd44780_config_t;
```

Public API:

```c
HD44780_Init(&lcd, &config);
HD44780_Clear(&lcd);
HD44780_SetCursor(&lcd, column, row);
HD44780_WriteString(&lcd, "text");
HD44780_WriteLine(&lcd, row, "text");
```

## Keypad Driver

Default thresholds are expressed like many Arduino examples, in a 10-bit range:

| Button | 10-bit range |
|---|---|
| RIGHT | `0-80` |
| UP | `81-200` |
| DOWN | `201-400` |
| LEFT | `401-600` |
| SELECT | `601-800` |
| NONE | `>800` |

Scale from a 12-bit ADC reading:

```text
value_10bit = raw_12bit * 1023 / 4095
```

Require three stable samples before accepting a button change.

## Config Tools Setup

The canonical `.mex` file is:

```text
src/lab_lcd/main/main.mex
```

Generated files should be in:

```text
cfg_tools/board/pin_mux.c
cfg_tools/board/pin_mux.h
cfg_tools/board/clock_config.c
cfg_tools/board/clock_config.h
cfg_tools/board/peripherals.c
cfg_tools/board/peripherals.h
cfg_tools/board/RTE_Device.h
```

The board adapter should use generated symbols such as:

```c
BOARD_INITPINS_LCD_RS_GPIO
BOARD_INITPINS_LCD_RS_PIN
BOARD_INITPINS_LCD_EN_GPIO
BOARD_INITPINS_LCD_D4_GPIO
BOARD_INITPINS_LCD_KEYPAD_A0_PORT
```

Avoid hard-coded GPIO numbers inside reusable drivers.

## What Can Go Wrong After Regeneration

The original project may have generated files in older locations such as:

```text
frdmmcxa153/main/pin_mux.*
frdmmcxa153/frdmmcxa153/clock_config.*
main/peripherals.*
```

After Config Tools regeneration, files can move to:

```text
cfg_tools/board/*
```

If CMake still refers to old files, Ninja may fail with a missing `clock_config.c` error. The fix is not in the LCD driver; the project metadata and CMake source list must be aligned with generated paths.

## Build and Verify

```powershell
cd src/lab_lcd/main
cmake --preset debug
cmake --build --preset debug
```

Check `debug/main_source_list.txt`. It should include:

```text
cfg_tools/board/clock_config.c
cfg_tools/board/pin_mux.c
cfg_tools/board/peripherals.c
```

It should not include deleted/generated files from old folders.

## Practical Test

1. Verify shield voltage compatibility.
2. Connect the shield to Arduino headers.
3. Flash the board.
4. Open Serial Monitor at `115200 8N1`.
5. Confirm the LCD startup message.
6. Press each button.
7. Compare LCD text with serial output.
8. Measure raw ADC values and adjust thresholds if needed.

## Common Pitfalls

- Do not connect 5 V A0 directly to MCXA153 ADC.
- Do not assume all LCD keypad shields use the same schematic.
- RESET is not part of the analog keypad.
- Do not use 10-bit thresholds directly on 12-bit values without scaling.
- Do not wait forever for ADC results.
- Do not compile old and regenerated `peripherals.c` files together.
- Run CMake configure again after Config Tools regeneration.

## Deliverable

Submit a project that:

- configures shield pins in `main.mex`;
- uses regenerated files from `cfg_tools/board`;
- initializes the LCD in 4-bit mode;
- detects RIGHT, UP, DOWN, LEFT, SELECT, and NONE;
- uses configurable thresholds and filtering;
- documents 3.3 V / 5 V electrical risks;
- builds with the `debug` preset.

