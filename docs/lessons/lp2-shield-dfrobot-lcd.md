---
title: "LP2 - DFRobot RGB LCD + Keypad Shield"
description: "Integrating the DFRobot RGBLCD1602 shield on FRDM-MCXA153: LPI2C0 for LCD and LPADC0 for buttons"
nav_order: 2
parent: Additional Demos
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_shield_lcd_keypad/main
---

# LP2 - DFRobot RGB LCD + Keypad Shield

**I2C 16x2 LCD, RGB backlight, and ADC keypad buttons**

---

| | |
|---|---|
| **Peripherals** | `LPI2C0`, `LPADC0`, `GPIO/pin mux`, serial console |
| **Duration** | 2 h |
| **Hardware** | FRDM-MCXA153 + DFRobot RGB LCD Keypad Shield / compatible RGBLCD1602 |
| **Project** | `src/lab_shield_lcd_keypad/main` |
| **Result** | LCD shows text and the current button: RIGHT, UP, DOWN, LEFT, SELECT, or NONE |

## Context

This lab adds an Arduino-style shield to FRDM-MCXA153. The shield has two independent parts:

1. a 16x2 HD44780-compatible LCD controlled through a DFRobot RGBLCD1602 I2C expander;
2. five buttons connected through a resistor ladder read on Arduino `A0`.

The important detail is that the shield uses the Arduino headers on the FRDM board. In this project, the LCD uses `LPI2C0`, and the buttons use `LPADC0`.

## Objectives

By the end of the lab you should be able to:

1. Identify the correct Arduino header pins on FRDM-MCXA153.
2. Configure `LPI2C0` for the DFRobot RGBLCD1602 LCD.
3. Configure `ADC0_A8` for the keypad resistor ladder.
4. Keep generated Config Tools files aligned with CMake.
5. Display button names on the LCD and in the serial console.
6. Calibrate ADC thresholds from measured raw values.

## Hardware and Pinout

| Shield function | Arduino header | MCXA153 pin | Peripheral |
|---|---:|---|---|
| LCD SDA | SDA | `P1_8` | `LPI2C0_SDA` |
| LCD SCL | SCL | `P1_9` | `LPI2C0_SCL` |
| Buttons | A0 | `P1_10` | `ADC0_A8` |
| Power | 3.3 V / 5 V depending on module | - | check levels |
| GND | GND | - | common ground |

DFRobot addresses:

```text
LCD controller: 0x3E
RGB controller: 0x60
```

Some Arduino documentation says SDA/SCL are `A4/A5`. On FRDM-MCXA153, use the Arduino SDA/SCL header pins mapped to `P1_8` and `P1_9`.

## Electrical Note

FRDM-MCXA153 uses 3.3 V logic. Before connecting the shield:

- check whether I2C pull-ups go to 3.3 V or 5 V;
- do not apply 5 V directly to MCXA153 GPIO/ADC pins;
- use a level shifter if the shield forces I2C to 5 V;
- keep A0 within the board's ADC range relative to `VDDA`.

## Code Structure

```text
src/lab_shield_lcd_keypad/main/
  main.c
  dfrobot_rgb_lcd.c/.h
  dfrobot_lcd_shield.c/.h
  dfrobot_lcd_shield_keypad.c/.h
  cfg_tools/board/pin_mux.c/.h
  main.mex
```

| File | Role |
|---|---|
| `dfrobot_rgb_lcd.c/.h` | DFRobot RGBLCD1602 protocol: init, clear, cursor, text, RGB backlight |
| `dfrobot_lcd_shield.c/.h` | FRDM board adapter: `LPI2C0` init, I2C write, address scan |
| `dfrobot_lcd_shield_keypad.c/.h` | `LPADC0` init, `ADC0_A8` read, debounce, raw-to-button mapping |
| `main.c` | Demo: initialize LCD, scan I2C, display current button |
| `main.mex` | Config Tools pin configuration |

## Config Tools Setup

In `main.mex`, verify:

```text
P1_8  -> LPI2C0_SDA  -> RGBLCD1602_ARDUINO_SDA
P1_9  -> LPI2C0_SCL  -> RGBLCD1602_ARDUINO_SCL
P1_10 -> ADC0_A8     -> LCD_KEYPAD_ARDUINO_A0
```

Generated files should be in:

```text
cfg_tools/board/pin_mux.c
cfg_tools/board/pin_mux.h
```

If Config Tools generates files elsewhere, align CMake so it compiles the same generated files that the tool updates.

Required components include:

```text
CONFIG_MCUX_COMPONENT_driver.cmsis_lpuart=y
CONFIG_MCUX_COMPONENT_driver.lpi2c=y
CONFIG_MCUX_COMPONENT_driver.lpadc=y
CONFIG_MCUX_COMPONENT_device.RTE=y
```

If Config Tools reports missing drivers or says files are not in the project, check `prj.conf`, `frdmmcxa153/prj.conf`, `cfg_tools/project_info.json`, `cfg_tools/cfg_require.json`, and CMake source lists.

## LCD over I2C

The LCD driver follows the DFRobot RGBLCD1602-style sequence:

1. initialize the LCD controller at `0x3E`;
2. initialize the RGB controller at `0x60`;
3. write text to the two rows;
4. change the RGB backlight.

Initial demo text:

```text
hello, world!
Key: NONE
```

## Keypad over Raw ADC

The buttons are connected through a resistor ladder to Arduino `A0`, which maps to `P1_10 / ADC0_A8`.

Measured raw values for the tested board/shield:

| Button | Approximate raw value |
|---|---:|
| RIGHT | `15` |
| UP | `19500` |
| DOWN | `38300` |
| LEFT | `58500` |
| SELECT | measure on your board |
| NONE | near upper end / open circuit |

Current thresholds:

```c
.right_max  = 9750U,
.up_max     = 28900U,
.down_max   = 48400U,
.left_max   = 62000U,
.select_max = 65000U,
```

If SELECT is not detected correctly, print the raw value and adjust `select_max`.

## Debounce

ADC values can wobble. The keypad driver accepts a new button only after the same candidate appears for three consecutive reads.

This is enough for a super-loop demo and prevents rapid wrong display updates.

## Build and Run

```powershell
cd C:\WORKSPACE\proiecte\ipcei-lab\src\lab_shield_lcd_keypad\main
cmake --build --preset debug
```

Expected serial output:

```text
FRDM-MCXA153 DFRobot RGBLCD1602 I2C demo
Using LPI2C0 on Arduino header SDA=P1_8 and SCL=P1_9
LCD address 0x3E, RGB address 0x60
I2C scan: 0x3E 0x60
RGBLCD1602 init sequence sent
ADC raw=... key=NONE
Button: RIGHT raw=15
```

The LCD should show `hello, world!` and the current key.

## Lab Exercise

1. Open `main.mex` and verify `P1_8`, `P1_9`, `P1_10`.
2. Compare `main.mex` with generated `cfg_tools/board/pin_mux.c`.
3. Build and flash the project.
4. Confirm the I2C scan shows `0x3E` and `0x60`.
5. Press every button and record raw ADC values.
6. Adjust SELECT threshold if needed.
7. Modify the LCD text to show your team initials.

## Common Problems

| Symptom | Likely cause | Fix |
|---|---|---|
| I2C scan lists all addresses | wrong SDA/SCL or floating bus | use `P1_8/P1_9` |
| LCD does not light | missing power or incompatible module | check VCC/GND and logic levels |
| LCD lights but no text | I2C init or contrast issue | verify `0x3E/0x60` scan |
| Wrong buttons | thresholds wrong | recalibrate using raw console values |
| SELECT appears as NONE | `select_max` too low | measure SELECT raw value |
| Config Tools creates files elsewhere | project metadata mismatch | check `.mex`, `project_info.json`, CMake |
| Missing driver warnings | incomplete metadata | restore required components and include required config files |

## Deliverable

Submit a project where:

1. LCD shows text on both rows;
2. RGB backlight changes;
3. serial console shows I2C scan and raw ADC values;
4. all buttons are detected or thresholds are documented;
5. `main.mex` contains the shield pins.

## Useful AI Prompt

```text
I am working on FRDM-MCXA153 with a DFRobot RGB LCD Keypad Shield.
LCD is on I2C: SDA=P1_8, SCL=P1_9, LPI2C0, addresses 0x3E and 0x60.
Buttons are on Arduino A0 = P1_10 / ADC0_A8, read with LPADC raw 16-bit.
Measured values: RIGHT=15, UP=19500, DOWN=38300, LEFT=58500, SELECT still to measure.
Check dfrobot_lcd_shield_keypad.c and adjust thresholds without changing the LCD driver.
Keep compatibility with MCUXpresso Config Tools and main.mex.
```

---

[<- L5: LPADC](../l5-lpadc) - [L7: LPI2C - P3T1755](../l7-lpi2c-p3t1755)

