---
title: "LP2 - DFRobot RGB LCD + Keypad Shield"
description: "Use the DFRobot RGBLCD1602 keypad shield to build a small LCD dinosaur game"
nav_order: 2
parent: Additional Demos
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_shield_lcd_keypad/main
---

# LP2 - DFRobot RGB LCD + Keypad Shield

**Configure the shield pins, verify the LCD and keypad, then build a small dinosaur runner game on the 16x2 display.**

---

| | |
|---|---|
| **Session** | Shield bring-up / LPI2C / LPADC |
| **Peripherals** | `LPI2C0`, `ADC0`, `FlexPWM0`, `LPUART0` |
| **Hardware** | FRDM-MCXA153 + DFRobot RGB LCD Keypad Shield / compatible RGBLCD1602 |
| **Console** | `LPUART0`, 115200 baud |
| **Main deliverable** | LCD dinosaur game with obstacles and jump control |

## Context

The DFRobot LCD keypad shield has two useful parts for this lab:

1. a 16x2 RGB LCD controlled over I2C;
2. five keypad buttons connected to Arduino `A0` through a resistor ladder.

The basic shield demo proves the LCD and keypad work. The final task is to turn that demo into a simple runner game, similar to the Google Chrome no-internet dinosaur animation.

## Pinout

Route the shield pins in MCUXpresso Config Tools as shown below.

| Shield signal | Arduino header | MCU pin | Function |
|---|---|---|---|
| LCD SDA | SDA | `P1_8` | `LPI2C0_SDA`, `Alt3` |
| LCD SCL | SCL | `P1_9` | `LPI2C0_SCL`, `Alt3` |
| Buttons | A0 | `P1_10` | `ADC0_A8` |
| Blue LED PWM | on-board LED | `P3_0` | `PWM0_A0` |
| Red LED GPIO | on-board LED | `P3_12` | GPIO output |

Use this Config Tools screenshot as the routing reference:

![MCUXpresso Config Tools pinout for the DFRobot LCD shield](../img/lab_shield_dfrobot_lcd/pin_config.png)

The reference screenshot should show `LPI2C0_SDA` on pin `2`, `LPI2C0_SCL` on pin `3`, `ADC0_A8` on pin `4`, `PWM0_A0` on pin `46`, and `ADC0_A14` on pin `32` if the older SW2 ADC route is still present.

## Objectives

By the end of this lab you should be able to:

1. configure `LPI2C0` for the LCD shield;
2. configure `ADC0_A8` for the keypad resistor ladder;
3. use generated peripheral macros such as `LPI2C0_PERIPHERAL` and `ADC0_PERIPHERAL`;
4. display text and custom game characters on a 16x2 LCD;
5. use a keypad button as a jump control;
6. implement a small obstacle-and-collision game loop.

## Config Tools Setup

Open the project `.mex` file and configure these items.

### LPI2C0

| Setting | Value |
|---|---|
| Peripheral | `LPI2C0` |
| Mode | Master |
| Baud rate | `100000` Hz |
| Pin config | 2-pin open-drain |
| SDA | `P1_8 / LPI2C0_SDA` |
| SCL | `P1_9 / LPI2C0_SCL` |

The LCD controller normally uses address `0x3E`. The RGB controller often uses `0x60`, but compatible modules may use `0x30`, `0x6B`, or `0x2D`.

### ADC0

Route `P1_10` as `ADC0_A8`. Use analog input mode. Do not enable digital pull-ups or pull-downs on the keypad pin.

### Update Code

After the pin, peripheral, and clock settings are correct, use **Update Code**. The startup should still call generated initialization once:

```c
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockFRO12M();
    BOARD_InitBootPeripherals();

    LED_RED_INIT(LOGIC_LED_OFF);
}
```

Do not manually edit generated files as the permanent solution.

## I2C Usage Pattern

Use the same SDK style as the P3T1755 I2C lesson: Config Tools initializes `LPI2C0`, and application/driver code only performs transfers.

```c
static bool LCD_I2C_Write(uint8_t address, const uint8_t *data, size_t length)
{
    lpi2c_master_transfer_t transfer = {0};

    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = address;
    transfer.direction = kLPI2C_Write;
    transfer.subaddress = 0U;
    transfer.subaddressSize = 0U;
    transfer.data = (uint8_t *)data;
    transfer.dataSize = length;

    return LPI2C_MasterTransferBlocking(LPI2C0_PERIPHERAL, &transfer) == kStatus_Success;
}
```

Do not reinitialize `LPI2C0` inside the LCD driver after `BOARD_InitHardware()`.

## Keypad Usage Pattern

Configure a software-triggered ADC command for channel `8`.

```c
#define KEYPAD_ADC_CHANNEL    8U
#define KEYPAD_ADC_COMMAND_ID 1U
#define KEYPAD_ADC_TRIGGER_ID 0U
```

Read the raw keypad value from the main loop, then map ranges to buttons.

Example measured values:

| Button | Approximate raw value |
|---|---:|
| RIGHT | `15` |
| UP | `19500` |
| DOWN | `38300` |
| LEFT | `58500` |
| SELECT | measure on your board |
| NONE | near upper end / open circuit |

Use serial output to calibrate thresholds for your shield.

## Basic Bring-Up

Before writing the game, verify the shield:

1. LCD initializes at `0x3E`.
2. RGB backlight changes color.
3. row 0 shows `hello, world!`.
4. row 1 shows `Key: NONE`.
5. pressing buttons updates row 1 and prints raw ADC values on serial.

Expected serial output:

```text
FRDM-MCXA153 DFRobot RGBLCD1602 I2C demo
LCD probe 0x3E: OK
Selected RGB address 0x60
RGBLCD1602 init sequence sent
ADC raw=... key=NONE
Button: RIGHT raw=15
```

## Main Task: LCD Dinosaur Game

Build a small dinosaur runner game on the 16x2 LCD.

Required behavior:

1. show a player/dinosaur character near the left side of the LCD;
2. move obstacles from right to left;
3. use one keypad button, such as UP or SELECT, to jump;
4. make the dinosaur return to the ground after the jump;
5. detect collision when an obstacle reaches the dinosaur while it is on the ground;
6. show score, distance, or survival time;
7. show a game-over state;
8. restart after a keypad button press.

Implementation hints:

- Use LCD custom glyphs for the dinosaur and obstacle if your driver supports them.
- A first version can use plain character cells such as `D`, `|`, and spaces.
- Keep game state in the main loop: player row, obstacle column, score, and game-over flag.
- Use a tick counter or delay to control game speed.
- Do not perform I2C LCD writes from interrupt handlers.

A simple display layout could be:

```text
Score: 0012
  D        |
```

When jumping:

```text
  D
           |
```

## Build And Run

Build the project with the existing CMake preset or MCUXpresso workflow:

```powershell
cmake --build --preset debug --clean-first
```

Flash the board and test in this order:

1. LCD text appears.
2. keypad raw values print on serial.
3. jump button works.
4. obstacles move.
5. collisions end the game.
6. restart works.

## Bring-Up Checklist

| Symptom | Likely cause | Fix |
|---|---|---|
| `LCD probe 0x3E: missing` | wrong SDA/SCL route or wiring | route `P1_8/P1_9`; check power and pull-ups |
| LCD backlight works but no text | LCD controller init or address issue | verify `0x3E`; check LCD writes |
| LCD text works but RGB does not | different RGB address | try `0x60`, `0x30`, `0x6B`, or `0x2D` |
| `kStatus_LPI2C_Busy` | SDA or SCL is not idle-high | check pull-ups/open-drain and shield voltage |
| ADC always reads NONE | wrong ADC pin/channel | route `P1_10` to `ADC0_A8`; use channel `8` |
| Wrong buttons | thresholds wrong | record raw values and recalibrate |
| Game flickers too much | too many LCD writes | update only changed cells or slow the game tick |
| Jump feels unresponsive | polling too slow or debounce too strict | tune loop timing and button detection |

## Deliverable

Submit a project where:

1. Config Tools routes `P1_8`, `P1_9`, and `P1_10` as shown in the pinout reference;
2. the LCD and keypad pass the basic bring-up test;
3. the final application is the LCD dinosaur game;
4. the dinosaur can jump over moving obstacles;
5. collisions produce a game-over state;
6. the game can restart from the keypad;
7. the serial console prints useful debug information such as raw ADC values or score.

**Task: upload the Gen AI chat to the evaluation platform**

---

[<- L5: LPADC](../l5-lpadc) - [L7: LPI2C - P3T1755](../l7-lpi2c-p3t1755)