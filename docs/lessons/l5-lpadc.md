---
title: "L5 - LPADC - Analog Voltage Measurement"
description: "LPADC0 setup, Arduino A0-A5 scanning, and voltage conversion"
nav_order: 6
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_adc/main
---

# L5 - LPADC - Analog Voltage Measurement

**MCUXpresso Config Tools, calibration, software trigger, and serial voltage output**

---

| | |
|---|---|
| **Session** | ADC |
| **Peripheral** | `ADC0 (LPADC)` |
| **Hardware** | FRDM-MCXA153, USB-C cable, analog sources up to 3.3 V |
| **Project** | `src/lab_adc/main` |

## Context and Motivation

The MCXA153 LPADC uses commands, triggers, and a result FIFO. This is different from simple ADC peripherals where selecting a channel and starting a conversion are handled by one register.

The application:

1. configures analog pins through MCUXpresso Config Tools;
2. initializes and calibrates `ADC0`;
3. uses high-resolution conversion;
4. scans several analog channels;
5. starts each conversion with a software trigger;
6. converts raw values to millivolts;
7. prints results to the serial console.

> Use `fsl_lpadc.h`. The `ADC16` driver used on other NXP families is not compatible with this project.

## Objectives

By the end of the lab you should be able to:

1. Identify ADC pins and LPADC channels for SW3 and Arduino A0-A5.
2. Configure analog pins in `.mex`.
3. Initialize and calibrate LPADC.
4. Configure a command and a software trigger.
5. Read conversion results from the FIFO.
6. Convert raw values to voltage.
7. Verify readings through Serial Monitor.

## Analog Input Map

| Application name | MCU pin | Board connector | LPADC channel |
|---|---|---|---|
| `SW3` | `P1_7` | SW3 / J1[1] | `ADC0_A23` |
| Arduino `A0` | `P1_10` | J4[2] | `ADC0_A8` |
| Arduino `A1` | `P1_12` | J4[4] | `ADC0_A10` |
| Arduino `A2` | `P1_13` | J4[6] | `ADC0_A11` |
| Arduino `A3` | `P2_0` | J4[8] | `ADC0_A0` |
| Arduino `A4` | `P3_31` | J4[10] | `ADC0_A12` |
| Arduino `A5` | `P3_30` | J4[12] | `ADC0_A13` |

Input voltage must remain between `0 V` and `VDDA`. In this lab, assume `VDDA = 3.3 V`.

## Config Tools Setup

Open:

```text
src/lab_adc/main/frdmmcxa153/main.mex
```

In **Pins**, configure the analog signals:

| Label | Signal |
|---|---|
| `SW3` | `ADC0_A23` |
| `ARDUINO_A0` | `ADC0_A8` |
| `ARDUINO_A1` | `ADC0_A10` |
| `ARDUINO_A2` | `ADC0_A11` |
| `ARDUINO_A3` | `ADC0_A0` |
| `ARDUINO_A4` | `ADC0_A12` |
| `ARDUINO_A5` | `ADC0_A13` |

For analog pins:

- disable pull resistors;
- disable the digital input buffer;
- select the analog mux function.

Generated files are expected under the board/config folder used by CMake.

## Enable the LPADC Driver

Ensure the project configuration enables LPADC:

```ini
CONFIG_MCUX_COMPONENT_driver.lpadc=y
```

Application code includes:

```c
#include "fsl_lpadc.h"
```

## Application Structure

### Channel Table

Use a table so the same read function can scan all channels:

```c
typedef struct
{
    const char *name;
    uint32_t channel;
} adc_input_t;

static const adc_input_t adc_inputs[] = {
    {"SW3", 23U},
    {"A0", 8U},
    {"A1", 10U},
    {"A2", 11U},
    {"A3", 0U},
    {"A4", 12U},
    {"A5", 13U},
};
```

### ADC Initialization

The ADC clock is derived from the 12 MHz FRO:

```c
CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
CLOCK_AttachClk(kFRO12M_to_ADC0);
RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);
```

Then initialize and calibrate:

```c
LPADC_GetDefaultConfig(&adc_config);
adc_config.powerLevelMode = kLPADC_PowerLevelAlt4;
adc_config.enableAnalogPreliminary = true;
adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
LPADC_Init(ADC0, &adc_config);

LPADC_DoOffsetCalibration(ADC0);
LPADC_DoAutoCalibration(ADC0);
```

### Command and Software Trigger

The lab uses command ID `1`, trigger ID `0`, and software trigger:

```c
LPADC_GetDefaultConvTriggerConfig(&trigger_config);
trigger_config.targetCommandId = ADC_COMMAND_ID;
trigger_config.enableHardwareTrigger = false;
LPADC_SetConvTriggerConfig(ADC0, ADC_TRIGGER_ID, &trigger_config);
```

### Read One Channel

```c
static uint16_t read_adc_channel(uint32_t channel)
{
    lpadc_conv_command_config_t command_config;
    lpadc_conv_result_t result;

    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.channelNumber = channel;
    command_config.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(ADC0, ADC_COMMAND_ID, &command_config);

    LPADC_DoSoftwareTrigger(ADC0, ADC_TRIGGER_MASK);
    while (!LPADC_GetConvResult(ADC0, &result))
    {
    }

    return result.convValue;
}
```

## Convert Raw Value to Voltage

High-resolution mode uses:

```text
raw = 0 ... 65535
```

For a `3300 mV` reference:

```text
voltage_mV = raw * 3300 / 65535
```

Rounded integer implementation:

```c
uint32_t voltage_mv = ((uint32_t)raw_value * 3300U + (65535U / 2U)) / 65535U;
```

Do not use a 12-bit denominator (`4095`) if the project is configured for high-resolution 16-bit output.

## Serial Output

Open the terminal at `115200 8N1`. Example output:

```text
ADC voltage measurement: SW3 and Arduino A0-A5
SW3     (ADC0_A23)     raw = 65410     voltage = 3.294 V
A0      (ADC0_A8)      raw = 32760     voltage = 1.650 V
```

Floating/unconnected inputs may show unstable values.

## Build

```powershell
cd src/lab_adc/main
cmake --preset debug
cmake --build --preset debug
```

## Practical Verification

1. Connect the board through MCU-Link.
2. Open Serial Monitor at `115200 8N1`.
3. Flash the application.
4. Apply a known voltage between `0 V` and `3.3 V` to one of `A0-A5`.
5. Connect the external source ground to board `GND`.
6. Compare the printed value with a multimeter.
7. Press SW3 and observe the change on `ADC0_A23`.

## Common Pitfalls

- Never apply negative voltage or voltage above `VDDA`.
- External equipment must share ground with the board.
- Disable the digital input buffer for analog pins.
- Do not enable internal pull-up/down resistors for external analog measurements.
- `ADC_COMMAND_ID` must not be zero.
- Trigger the conversion before reading the result.
- Arduino `A0` is not LPADC channel 0; it maps to `ADC0_A8`.
- Floating inputs can look random.
- Verify the active `.mex` file used by the build.

## Exercises

1. Compute theoretical microvolts per bit for `VDDA = 3.3 V`.
2. Average 16 samples per channel.
3. Print only channels that changed by more than `10 mV`.
4. Replace the blocking delay with a periodic timer.
5. Estimate measurement error using a reference multimeter.

## Deliverable

Submit an application that:

- configures SW3 and Arduino `A0-A5` in `.mex`;
- builds with the LPADC driver;
- calibrates ADC;
- scans all seven channels;
- prints raw and voltage values;
- documents the pin/channel map and conversion formula.

---

[<- L4: PWM with FlexPWM](../l4-timer-pwm) - [L6: LPSPI - TFT LCD ILI9341 ->](../l6-lpspi-tft-lcd)

