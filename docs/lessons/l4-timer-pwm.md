---
title: "L4 - PWM with FlexPWM"
description: "RGB LED fading with FlexPWM0, pin muxing, and AI-assisted extension"
nav_order: 5
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_pwm/pwm_control
---

# L4 - PWM with FlexPWM

**RGB LED fading with FlexPWM0**

---

| | |
|---|---|
| **Session** | Timer + PWM |
| **Peripheral** | `FlexPWM0` |
| **Hardware** | FRDM-MCXA153, RGB LED, Serial Monitor, optional oscilloscope |
| **Project** | `src/lab_pwm/pwm_control` |

## Context and Motivation

This lab uses `FlexPWM0`, the dedicated PWM peripheral. Unlike software GPIO toggling, FlexPWM generates a waveform in hardware. The CPU updates duty cycle values while the peripheral keeps a stable frequency.

On FRDM-MCXA153, the RGB LED pins can be routed to FlexPWM:

| LED | Pin / Function |
|---|---|
| Red | `P3_12 / PWM0_X0` |
| Green | `P3_13 / PWM0_X1` |
| Blue | `P3_0 / PWM0_A0` |

The starter controls red and green with PWM. The lab extension adds blue PWM and an RGB color wheel.

## Objectives

By the end of the lab you should be able to:

1. Explain the difference between GPIO blinking and hardware PWM.
2. Configure pins for `FlexPWM0` instead of GPIO.
3. Initialize FlexPWM with `PWM_GetDefaultConfig()`, `PWM_Init()`, and `PWM_SetupPwm()`.
4. Update duty cycle without reinitializing the peripheral.
5. Use Serial Monitor to inspect duty cycle values.
6. Use AI to extend a working starter safely.

## FlexPWM Starter

The starter controls:

- red LED: `P3_12 / PWM0_X0 / kPWM_Module_0 / kPWM_PwmX`;
- green LED: `P3_13 / PWM0_X1 / kPWM_Module_1 / kPWM_PwmX`.

The blue LED starts as GPIO/off and is added later.

### Pin Mux

Pins must be switched from GPIO to FlexPWM alternate function:

```text
P3_12 -> PWM0_X0 -> kPORT_MuxAlt5
P3_13 -> PWM0_X1 -> kPORT_MuxAlt5
```

If a pin remains GPIO, FlexPWM registers may update correctly while the LED does nothing.

`FLEXPWM0` must also be released from reset:

```c
RESET_ReleasePeripheralReset(kFLEXPWM0_RST_SHIFT_RSTn);
```

### FlexPWM Initialization

The starter uses two submodules:

```c
PWM_GetDefaultConfig(&pwmConfig);
PWM_Init(FLEXPWM0, kPWM_Module_0, &pwmConfig);
PWM_Init(FLEXPWM0, kPWM_Module_1, &pwmConfig);
```

The signal uses:

- `kPWM_PwmX`;
- `kPWM_EdgeAligned`;
- `1000 Hz`;
- `kPWM_LowTrue`, because the LEDs are active-low.

After setup, enable outputs, load buffered registers with `LDOK`, then start the timer:

```c
PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
PWM_StartTimer(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1);
```

### Duty Cycle Updates

Duty cycle is updated without reinitializing the timer:

```c
PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_EdgeAligned, red_duty);
PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_EdgeAligned, green_duty);
PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
```

`PWM_UpdatePwmDutycycle()` writes buffered values. `PWM_SetPwmLdok()` makes them active.

## Important Debug Discovery: Fault Disable Mapping

During bring-up, the UART showed duty cycle updates, and GPIO self-test proved the LEDs worked, but PWM output stayed off. The fix was clearing the fault disable map:

```c
PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_faultchannel_0, 0U);
PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_faultchannel_0, 0U);
```

If a fault input is mapped as active, FlexPWM can disable the output even while the timer is running. This lab does not use fault inputs, so the mapping is cleared explicitly.

## Diagnostic Self-Test

A short GPIO self-test before switching pins to PWM helps isolate problems:

```c
PORT_SetPinMux(BOARD_INITPINS_LED_RED_PORT, BOARD_INITPINS_LED_RED_PIN, kPORT_MuxAlt0);
PORT_SetPinMux(BOARD_INITPINS_LED_GREEN_PORT, BOARD_INITPINS_LED_GREEN_PIN, kPORT_MuxAlt0);

LED_RED_INIT(LOGIC_LED_ON);
LED_GREEN_INIT(LOGIC_LED_ON);
SDK_DelayAtLeastUs(500000U, SystemCoreClock);

LED_RED_OFF();
LED_GREEN_OFF();

PORT_SetPinMux(BOARD_INITPINS_LED_RED_PORT, BOARD_INITPINS_LED_RED_PIN, kPORT_MuxAlt5);
PORT_SetPinMux(BOARD_INITPINS_LED_GREEN_PORT, BOARD_INITPINS_LED_GREEN_PIN, kPORT_MuxAlt5);
```

If the LEDs fail during GPIO self-test, the problem is pin mapping or hardware. If GPIO works but PWM does not, debug FlexPWM configuration.

## AI Assistant Prompts

### Prompt: Explain the FlexPWM Starter

```text
I have an MCUXpresso SDK project for FRDM-MCXA153.
The starter uses FlexPWM0:
- red: P3_12 / PWM0_X0 / kPWM_Module_0 / kPWM_PwmX
- green: P3_13 / PWM0_X1 / kPWM_Module_1 / kPWM_PwmX
Explain for students:
1. pin muxing to FlexPWM;
2. PWM_GetDefaultConfig, PWM_Init, PWM_SetupPwm;
3. why kPWM_PwmX and kPWM_LowTrue are used;
4. why PWM_SetPwmLdok is required after duty updates;
5. why PWM_SetupFaultDisableMap(..., 0U) is needed.
Do not rewrite all code; explain the existing design.
```

### Prompt: Add Blue PWM and Color Wheel

```text
I have an MCUXpresso SDK project for FRDM-MCXA153.
Current PWM channels:
- red: P3_12 / PWM0_X0 / kPWM_Module_0 / kPWM_PwmX
- green: P3_13 / PWM0_X1 / kPWM_Module_1 / kPWM_PwmX
Add blue LED PWM:
- blue: P3_0 / PWM0_A0 / kPWM_Module_0 / kPWM_PwmA
- pin mux must use kPORT_MuxAlt5.
Requirements:
1. keep the code simple for students;
2. add blue PWM setup;
3. keep PWM_SetupFaultDisableMap(..., 0U), including for blue;
4. implement a red -> green -> blue -> red color wheel;
5. print red, green, and blue duty cycles to Serial Monitor;
6. explain what must change in .mex so P3_0 appears as FlexPWM0 A,0.
```

## Checklist for the Blue LED Extension

In code:

- `PWM_InitLedChannel()` should accept the PWM channel (`kPWM_PwmX` or `kPWM_PwmA`).
- Blue uses `kPWM_Module_0` and `kPWM_PwmA`.
- Blue also needs `PWM_SetupFaultDisableMap(..., kPWM_PwmA, ..., 0U)`.
- `RGB_SetDuty()` updates red, green, and blue.
- `PWM_SetPwmLdok()` is called after updates.

In pin mux:

- `P3_0` must be `PWM0_A0`, not `GPIO3,0`.
- The generated pin mux should use `kPORT_MuxAlt5` for `P3_0`.

In `.mex`:

```xml
<pin peripheral="FlexPWM0" signal="A, 0" pin_num="46" pin_signal="P3_0/WUU0_IN22/TRIG_IN0/CT_INP16/PWM0_A0">
```

## Common Pitfalls

- `PWM0_X0` and `PWM0_X1` use `kPWM_PwmX`, not `kPWM_PwmA` or `kPWM_PwmB`.
- The pin must be routed to FlexPWM. GPIO mux will not output PWM.
- Duty updates require `PWM_SetPwmLdok()`.
- Fault mapping can disable PWM output.
- The FRDM RGB LEDs are active-low, so polarity matters.
- Avoid 0% and 100% edge cases during fade; use a range such as `1..99`.

## Deliverable

Submit:

1. working red/green FlexPWM starter;
2. Serial Monitor output showing duty cycle changes;
3. blue PWM extension;
4. RGB color wheel;
5. `.mex` checked visually in Config Tools.

---

[<- L3: External Interrupts and NVIC](../l3-intreruperi) - [L5: LPADC - Analog Voltage Measurement ->](../l5-lpadc)

