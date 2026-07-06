---
title: "L5 - LPADC - Analog Voltage Measurement"
description: "Configure ADC0 with MCUXpresso Config Tools, then read and print the SW2 voltage"
nav_order: 6
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_new_project_configured/configured
---

# L5 - LPADC - Analog Voltage Measurement

**Configure ADC0 in MCUXpresso Config Tools, then implement a small SW2 voltage-measurement application**

---

| | |
|---|---|
| **Session** | ADC |
| **Peripheral** | `ADC0 (LPADC)` |
| **Hardware** | FRDM-MCXA153, USB-C cable |
| **Project** | `src/lab_new_project_configured/configured` |
| **Config file** | `src/lab_new_project_configured/configured/frdmmcxa153/led_blinky.mex` |
| **Application file** | `src/lab_new_project_configured/configured/led_blinky.c` |
| **Measured signal** | SW2 voltage on `ADC0_A14` |

## Context and Motivation

The MCXA153 LPADC is configured from several pieces that must agree with each other: the ADC peripheral instance, its clock source, the pin mux, the generated board files, the SDK driver component, and the application code that starts conversions.

In this lab, treat MCUXpresso Config Tools as the source of truth for hardware setup. First configure `ADC0`, its clock, and the SW2 pin. Then use the agent workflow from [LP2 - From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent) to generate and review the application code.

> Use `fsl_lpadc.h`. The `ADC16` driver used on other NXP families is not compatible with this project.

## Objectives

By the end of the lab you should be able to:

1. Configure `ADC0` in the Peripherals tool.
2. Enable the clock path used by `ADC0`.
3. Use the board schematic to identify the analog input connected to SW2.
4. Configure `P1_29` as an analog ADC input in the Pins tool.
5. Enable the LPADC SDK driver component.
6. Generate application code with a specification-first agent workflow.
7. Read a raw LPADC value, convert it to voltage, and print both values on the serial console.

## Open the Project Configuration

Open the configured project in MCUXpresso for VS Code, then open the active `.mex` file:

```text
src/lab_new_project_configured/configured/frdmmcxa153/led_blinky.mex
```

Make sure Config Tools is opened from the same project folder that you build with CMake. If Config Tools writes generated files into another folder, the application can compile stale pin, clock, or peripheral configuration.

## Configure ADC0 in Peripherals

Open the **Peripherals** tool and configure the `ADC0` LPADC peripheral. The generated configuration should initialize the ADC instance used by the application.

![Configuring ADC0 in the Peripherals tool](../img/lab_adc/config_peripheral.png)

*Figure 1. ADC0 configured from the Peripherals tool. This is where the project-level peripheral setup is generated.*

The application will still trigger conversions from C code, but the peripheral must exist in the generated configuration and the matching SDK driver must be available to the build.

## Configure the ADC0 Clock

Switch to the **Clocks** tool and configure the clock used by `ADC0`.

![Configuring the ADC0 clock in the Clocks tool](../img/lab_adc/config_clock.png)

*Figure 2. ADC0 clock configuration. Peripheral configuration and clock configuration must agree before generated setup is complete.*

The application in `configured/led_blinky.c` relies on generated project initialization instead of hand-writing the ADC clock setup in the root C file:

```c
BOARD_InitHardware();
ADC0_SW2_InitCommandTrigger();
```

Keep the generated clock setup and the application setup consistent. If you change the clock source in Config Tools, review the generated board files and the application initialization sequence.

## Check the Board Schematic

Use the FRDM-MCXA153 board schematic to identify the SW2 signal. In this lab, SW2 is used as the analog input that will be read by `ADC0`.

![SW2 connection in the board schematic](../img/lab_adc/config_pin_schematic.png)

*Figure 3. SW2 schematic connection. The lab reads the raw ADC sample from the SW2 pin through the LPADC.*

For this project, SW2 is connected to:

| Signal | MCU pin | LPADC channel |
|---|---|---|
| `SW2` | `P1_29` | `ADC0_A14` |

## Configure P1_29 in Pins

Open the **Pins** tool and configure `P1_29` for the ADC input function.

![Configuring P1_29 in the Pins tool](../img/lab_adc/config_pin.png)

*Figure 4. P1_29 configured as the analog input used by ADC0_A14.*

For an analog input, use the pin settings expected by the ADC:

1. Select the ADC analog function for `P1_29`.
2. Disable internal pull-up and pull-down resistors.
3. Disable the digital input buffer when the tool exposes that option.
4. Apply the generated code update only after reviewing the changed files.

## Enable the LPADC Driver

The CMake project must also enable the matching SDK driver component. Check the board-specific project configuration:

```text
src/lab_new_project_configured/configured/frdmmcxa153/prj.conf
```

Ensure it contains:

```ini
CONFIG_MCUX_COMPONENT_driver.lpadc=y
```

The application code uses the generated board/peripheral headers and the lab UART helper:

```c
#include "board.h"
#include "app.h"
#include "peripherals.h"
#include <stdbool.h>

#include "app/uart.h"
```

If Config Tools reports that the ADC driver is missing from the toolchain or IDE project, verify `frdmmcxa153/prj.conf`, rerun CMake, and reopen the `.mex` file.

## Write the Application with Agents

Use the same specification-first flow from [LP2 - From Vibe Coding to Coding Agent](../lp2-from-vibe-coding-to-coding-agent). The idea is to ask one agent to inspect the generated configuration and produce a specification, then ask a second agent to implement only the application source changes.

### First Agent: Planning

```text
Use `.agents/mcux-config-to-main-spec.prompt.md`.

APPLICATION_TASK: read the raw ADC sample on SW2 pin (ADC0_A14) using the ADC configured in the project, convert it to voltage, and print both values on the console.

Use the current working directory as PROJECT_ROOT.

Return the specification only. Do not edit files.
```

Review the returned specification. Check that it found the active project root, the ADC instance, the generated configuration files, the console setup, and the correct LPADC channel.

### Second Agent: Coding

```text
Use `.agents/mcux-main-codegen-from-spec.prompt.md` with the validated specification from the first agent.

Use the current working directory as PROJECT_ROOT.

Apply the code changes only to the active root C file.
Do not edit .mex or generated board files.
```

The generated code should initialize the board, configure the ADC command/trigger for channel `14`, read the FIFO result, convert it to millivolts, and print both the raw value and voltage on the console.

### Third Agent: Review and Debug

If the code does not build or the behavior does not match the specification:

```text
Use `.agents/mcux-feedback-to-change-spec.prompt.md` with the change specification above.

FEEDBACK: <your observations>.

INTENDED_BEHAVIOR: <specification>.

Use the current working directory as PROJECT_ROOT.
Do not edit files. Return the change specification only.
```

Then run the coding agent again with the change specification.

## Application Code Reference

Use `src/lab_new_project_configured/configured/led_blinky.c` as the reference implementation for the application-owned ADC code. The snippets below keep only the ADC-related parts and add the voltage conversion used by this lesson.

The application constants identify the SW2 ADC channel, command, trigger, and voltage conversion parameters:

```c
#define ADC_SW2_CHANNEL 14U
#define ADC_COMMAND_ID 1U
#define ADC_TRIGGER_ID 0U
#define ADC_REFERENCE_MV 3300U
#define ADC_FULL_SCALE 65535U
```

The ADC command and software trigger are configured once after board initialization. Notice that this code uses `ADC0_PERIPHERAL`, the symbol generated by Config Tools, instead of hard-coding `ADC0`:

```c
static void ADC0_SW2_InitCommandTrigger(void)
{
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_trigger_config_t triggerConfig;

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = ADC_SW2_CHANNEL;
    LPADC_SetConvCommandConfig(ADC0_PERIPHERAL, ADC_COMMAND_ID, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = ADC_COMMAND_ID;
    LPADC_SetConvTriggerConfig(ADC0_PERIPHERAL, ADC_TRIGGER_ID, &triggerConfig);
}
```

Each sample is taken by issuing a software trigger and waiting for one FIFO result:

```c
static uint32_t ADC0_SW2_GetSample(void)
{
    lpadc_conv_result_t result;
    LPADC_DoSoftwareTrigger(ADC0_PERIPHERAL, 1UL << ADC_TRIGGER_ID);
    while (!LPADC_GetConvResult(ADC0_PERIPHERAL, &result));
    return (uint32_t)result.convValue;
}
```

Convert the raw 16-bit ADC value to millivolts with rounded integer arithmetic:

```c
static uint32_t ADC0_SW2_ConvertToMv(uint32_t rawValue)
{
    return (rawValue * ADC_REFERENCE_MV + (ADC_FULL_SCALE / 2U)) / ADC_FULL_SCALE;
}
```

The print path follows the reference implementation's UART helper calls, with voltage output added after the raw ADC value:

```c
uint32_t adc_value = ADC0_SW2_GetSample();
uint32_t voltage_mv = ADC0_SW2_ConvertToMv(adc_value);

LPUART0_WriteString("counter=");
LPUART0_WriteU32(printCounter);
LPUART0_WriteString("\tADC0_A14=");
LPUART0_WriteU32(adc_value);
LPUART0_WriteString("\r\n");
printCounter++;
```

## Convert Raw Value to Voltage

The ADC helper returns a raw sample:

```text
raw = 0 ... 65535
```

For a 16-bit result and a `3300 mV` reference:

```text
voltage_mV = raw * 3300 / 65535
```

Use the rounded implementation shown above, and do not use a 12-bit denominator (`4095`) if the project is configured for high-resolution 16-bit output.

## Build and Run

Build the project:

```powershell
cd src/lab_new_project_configured/configured
cmake --preset debug
cmake --build --preset debug
```

Open the terminal at `115200 8N1`, flash the application, and watch the printed ADC reading.

Example output:

```text
counter=0    ADC0_A14=42110    voltage=2.120 V
counter=1    ADC0_A14=42088    voltage=2.119 V
```

The exact value depends on the board state and the SW2 circuit. Press and release SW2 while watching the serial output and confirm that the raw value changes.

## Practical Verification

1. Connect the board through MCU-Link.
2. Open Serial Monitor at `115200 8N1`.
3. Flash the application.
4. Observe the printed `ADC0_A14` raw value and voltage.
5. Press SW2 and confirm that the reading changes.
6. If you measure the pin with external equipment, connect the external ground to board `GND`.

## Common Pitfalls

- Never apply negative voltage or voltage above `VDDA`.
- External equipment must share ground with the board.
- Do not enable internal pull-up or pull-down resistors for external analog measurements unless the circuit requires them.
- Disable the digital input buffer for analog pins when available.
- `ADC_COMMAND_ID` must not be zero.
- Trigger the conversion before reading the result FIFO.
- `SW2` maps to `ADC0_A14`; do not assume the switch is LPADC channel `0`.
- Verify that Config Tools updated the generated files used by the active CMake build.

## Exercises

1. Compute theoretical microvolts per bit for `VDDA = 3.3 V`.
2. Average 16 samples from `ADC0_A14` before printing.
3. Add a millivolt conversion and print only when the voltage changes by more than `10 mV`.
4. Add a second analog input and read both channels with the same helper function.
5. Compare the serial output with a multimeter measurement.

## Deliverable

Submit an application that:

1. configures `ADC0` in the Peripherals tool;
2. configures the `ADC0` clock in the Clocks tool;
3. configures `P1_29` / `ADC0_A14` in the Pins tool;
4. builds with the LPADC driver enabled;
5. reads SW2 through LPADC;
6. converts raw values to voltage;
7. prints raw `ADC0_A14` values and voltage on the serial console.

**Task: upload the Gen AI chat to the evaluation platform**

---

[<- L4: PWM with FlexPWM](../l4-timer-pwm) - [L6: LPSPI - TFT LCD ILI9341 ->](../l6-lpspi-tft-lcd)
