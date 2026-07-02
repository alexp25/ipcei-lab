---
title: "L8 - Integration - Complete FSM System"
description: "All peripherals, super-loop architecture, and show-and-tell"
nav_order: 9
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_integration/main
---

# L8 - Integration - Complete FSM System

**All peripherals, super-loop, and show-and-tell**

---

| | |
|---|---|
| **Session** | Integration |
| **Topic** | `GPIO`, `UART`, `IRQ`, `SysTick/FlexPWM`, `LPADC`, `I2C/I3C`, `FSM` |
| **Duration** | 3 h + final demo preparation |
| **Hardware** | FRDM-MCXA153 with the connections used in L1-L7 |

## Context and Motivation

The integration session checks whether peripherals learned separately can work together in one firmware project. This is the most realistic embedded exercise in the course: integration bugs are usually caused by timing, shared state, blocking I/O, or unclear peripheral ownership.

An AI assistant can generate a conceptually correct finite-state machine, but it may miss embedded constraints such as atomic flag access between ISRs and the main loop.

## Objectives

By the end of the lab you should be able to:

1. Run GPIO, UART, IRQ, SysTick/FlexPWM, LPADC, and I2C/I3C together.
2. Coordinate the application with a finite-state machine without an RTOS.
3. Use a super-loop with `volatile` flags set by ISRs.
4. Identify integration conflicts such as blocking UART and shared variables.
5. Present a short live demo and an AI reflection.

## Integrated Demo Architecture

Suggested active peripherals:

| Peripheral | Role | Channel / Pin |
|---|---|---|
| I2C/I3C-compatible bus | read P3T1755 temperature | on-board sensor |
| `LPADC0` | variable threshold from potentiometer | Arduino `A0` / `ADC0_A8` |
| `FlexPWM0` module 0 | red LED intensity | `PWM0_X0` |
| `FlexPWM0` module 1 | green LED intensity | `PWM0_X1` |
| GPIO | blue LED cooling indicator | board LED blue |
| GPIO IRQ | SW3 start/stop monitoring | falling edge |
| SysTick | 1 ms tick, 500 ms sampling | system clock |
| `LPUART0` | state, temperature, threshold log | 115200 baud |

## FSM

```text
STATE_IDLE --SW3--> STATE_MONITORING
STATE_MONITORING --SW3--> STATE_IDLE
STATE_MONITORING --temp >= threshold--> STATE_ALERT
STATE_ALERT --temp < threshold - hysteresis--> STATE_COOLING
STATE_COOLING --temp < threshold - hysteresis--> STATE_MONITORING
STATE_COOLING --temp >= threshold--> STATE_ALERT
```

```c
typedef enum
{
    STATE_IDLE = 0,
    STATE_MONITORING,
    STATE_ALERT,
    STATE_COOLING
} app_state_t;
```

## LED Behavior

| State | Red PWM | Green PWM | Blue GPIO |
|---|---|---|---|
| `IDLE` | 0% | 0% | OFF |
| `MONITORING` | proportional to temperature | inverse of red | OFF |
| `ALERT` | 100% | 0% | OFF |
| `COOLING` | proportional to temperature | inverse of red | ON |

## Variable ADC Threshold

Use the potentiometer on Arduino `A0` to set a threshold from `20 C` to `40 C`:

```c
#define THRESHOLD_MIN_CELSIUS      20.0F
#define THRESHOLD_SPAN_CELSIUS     20.0F
#define COOLING_HYSTERESIS_CELSIUS 2.0F

float adc_raw_to_threshold(uint16_t rawValue)
{
    return THRESHOLD_MIN_CELSIUS +
           ((float)rawValue * THRESHOLD_SPAN_CELSIUS) / 65535.0F;
}
```

## Super-Loop with Atomic Flag Reads

ISRs set flags. The main loop consumes them atomically:

```c
volatile bool g_buttonPressed;
volatile bool g_sampleTick;

void BOARD_SW3_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
    {
        GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);
        g_buttonPressed = true;
    }
}

static bool take_flag(volatile bool *flag)
{
    bool value;
    __disable_irq();
    value = *flag;
    *flag = false;
    __enable_irq();
    return value;
}

while (1)
{
    bool sampleTick = take_flag(&g_sampleTick);
    bool buttonPressed = take_flag(&g_buttonPressed);

    if (sampleTick)
    {
        /* read ADC, read temperature, update FSM, update LEDs, print log */
    }

    if (buttonPressed)
    {
        /* process SW3 event */
    }

    __WFI();
}
```

## AI Assistant Prompt: Integrated FSM Skeleton

```text
FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK.
All peripherals active together:
- P3T1755 temperature read every 500 ms;
- LPUART0 logging;
- FlexPWM0 red/green LED output;
- LPADC0 channel 8 on Arduino A0 for a variable threshold;
- GPIO IRQ SW3 falling edge for start/stop monitoring.
Use:
typedef enum { STATE_IDLE, STATE_MONITORING, STATE_ALERT, STATE_COOLING } app_state_t;
Task: generate only the C skeleton for the FSM, not full drivers.
Architecture: super-loop, volatile flags set from ISRs, no RTOS.
Generate:
- global volatile flags;
- static bool take_flag(volatile bool *flag) with __disable_irq()/__enable_irq();
- process_fsm(app_state_t *state, ...);
- main loop using __WFI();
- comments about shared state protection.
```

## AI Assistant Prompt: Blocking UART vs ISR Latency

```text
FRDM-MCXA153 integrated system with a super-loop and __WFI().
Problem: LPUART_WriteBlocking() / PRINTF() can block the CPU while sending text at 115200 baud.
Impact: SW3 and SysTick ISRs still run, but main loop processing is delayed.
Compare:
1. UART ring buffer + TX empty interrupt;
2. LPUART DMA;
3. logging less often or only on state changes.
Which solution is best for a no-RTOS MCX A153 lab project and why?
```

## Common Pitfalls

- AI may suggest a different timer/PWM peripheral than the one used by the project.
- AI may forget atomic reads for flags shared between ISR and main.
- Do not perform slow I2C, ADC, or printing directly inside an ISR.
- `PRINTF()` can block; log less often or move UART output to a buffered design.
- Make sure every generated file used by Config Tools is also compiled by CMake.
- Run a clean build after regenerating Config Tools files.
- Keep one clear owner for every peripheral clock.

## Expected Console Behavior

```text
FRDM-MCXA153 integration demo
Press SW3 to start/stop monitoring. A0 sets threshold from 20.0 C to 40.0 C.
state=IDLE       temp=N/A     threshold=30.00 C adc=32767
state=MONITORING temp=25.50 C threshold=30.00 C adc=32767
state=ALERT      temp=31.20 C threshold=30.00 C adc=32767
state=COOLING    temp=27.80 C threshold=30.00 C adc=32767
```

## Deliverable

Submit:

1. a working integrated system;
2. FSM diagram or transition table;
3. explanation of super-loop and ISR flags;
4. short live demo plan;
5. AI reflection: what the assistant got wrong and how you fixed it.

---

[<- L7: LPI2C - P3T1755 Temperature Sensor](../l7-lpi2c-p3t1755) - [L9: Testing, Documentation and Final Presentation ->](../l9-testing-docs)

