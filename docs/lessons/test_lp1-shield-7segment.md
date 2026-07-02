---
title: "Test LP1 - Integration System with FSM"
description: "All peripherals, super-loop architecture, and final show-and-tell"
nav_order: 9
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_shield_7segment/main
---

# Test LP1 - Integration System with FSM

**All peripherals, super-loop, and final demonstration**

---

| | |
|---|---|
| **Session** | Integration |
| **Topic** | `GPIO`, `UART`, `IRQ`, `Timer/PWM`, `ADC`, `LPI2C`, `FSM` |
| **Hardware** | FRDM-MCXA153 with connections from previous labs |

## Context and Motivation

The integration session tests whether the peripherals learned separately can run together in one firmware project. This is close to real embedded work: integration problems are often caused by timing, shared state, blocking I/O, and peripheral ownership.

An AI assistant can generate a plausible FSM, but it may miss embedded details such as shared variables between ISRs and the main loop.

## Objectives

By the end of the lab you should be able to:

1. Combine GPIO, UART, interrupts, PWM, ADC, and I2C in one project.
2. Coordinate behavior with a finite-state machine.
3. Use a super-loop architecture without an RTOS.
4. Identify integration conflicts such as blocking UART vs ISR latency.
5. Present a short live demo and explain one AI mistake you found.

## Suggested Integrated System

Build a small monitoring application:

- read P3T1755 temperature through `LPI2C0` once per second;
- log status over `LPUART0`;
- drive an RGB LED with PWM according to temperature;
- use `SW3` as an interrupt-driven alert/acknowledge button;
- read an analog threshold from LPADC;
- run a finite-state machine in the super-loop.

Example states:

```c
typedef enum
{
    STATE_IDLE,
    STATE_MONITORING,
    STATE_ALERT,
    STATE_COOLING
} app_state_t;
```

## Super-Loop Pattern

ISRs should set flags. The main loop should process them:

```c
volatile bool g_buttonPressed;
volatile bool g_timerTick;

while (1)
{
    if (g_timerTick)
    {
        g_timerTick = false;
        sample_sensors();
    }

    if (g_buttonPressed)
    {
        g_buttonPressed = false;
        handle_button_event();
    }

    process_fsm(&state);
}
```

For shared variables modified by ISRs and read in `main()`, use `volatile` and protect multi-byte or multi-field updates when needed.

## AI Assistant Prompt: FSM Skeleton

```text
FRDM-MCXA153, all peripherals active together:
LPI2C0 P3T1755 at 1 Hz, LPUART0 logging, PWM LED, LPADC threshold input, GPIO IRQ SW3.
Task: generate only the C skeleton for a finite-state machine, not the full implementation.
Use:
typedef enum { STATE_IDLE, STATE_MONITORING, STATE_ALERT, STATE_COOLING } app_state_t;
Architecture: super-loop with volatile flags set from ISRs, no RTOS.
Generate:
- required headers;
- global data structure;
- void process_fsm(app_state_t *state);
- comments about what must be protected with DisableIRQ/EnableIRQ.
```

## AI Assistant Prompt: UART Blocking vs ISR Latency

```text
FRDM-MCXA153 integrated system.
Problem: LPUART_WriteBlocking() can block the CPU while sending text at 115200 baud.
Impact: SW3 ISR responsiveness and timing-sensitive work may suffer during long logs.
Compare:
1. UART ring buffer + TX-empty interrupt;
2. LPUART DMA;
3. logging less often.
Which solution is best for a no-RTOS MCX A153 project and why?
```

## Common Pitfalls

- `LPUART_WriteBlocking()` can stall the CPU during long messages.
- AI may forget atomic protection for variables shared between ISR and `main()`.
- Do not do slow I2C, ADC, or printing work directly inside an ISR.
- Keep a clear owner for each peripheral clock and generated configuration.
- If using DMA, check channel conflicts between peripherals.
- Log less frequently than the main loop runs.

## Deliverable

Submit:

1. a working integrated firmware project;
2. FSM diagram or state table;
3. short live demo plan;
4. explanation of one integration problem you solved;
5. reflection on what AI suggested incorrectly and how you verified/fixed it.

---

[<- L7: LPI2C - P3T1755 Temperature Sensor](../l7-lpi2c-p3t1755)

