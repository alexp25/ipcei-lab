---
title: "L2 - LPUART - Serial Communication"
description: "USB virtual COM through MCU-Link and printf retargeting"
nav_order: 3
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_uart/main
---

# L2 - LPUART - Serial Communication

**USB virtual COM through MCU-Link and retargeted `printf`**

---

| | |
|---|---|
| **Session** | UART |
| **Peripheral** | `LPUART0`, `VCOM` |
| **Hardware** | FRDM-MCXA153, USB Type-C MCU-Link port, serial terminal on PC |

## Context and Motivation

On FRDM-MCXA153, `LPUART0` is connected to the on-board MCU-Link interface, which exposes a USB virtual COM port. No external USB-to-UART adapter is required.

This lab introduces serial logging, polling-based UART I/O, `printf` retargeting, and common clock-source problems.

## Objectives

By the end of the lab you should be able to:

1. Configure `LPUART0` for `115200 8N1`.
2. Print text to the VS Code Serial Monitor or another terminal.
3. Retarget `printf` through `LPUART_WriteBlocking()`.
4. Explain why the UART source clock and divider matter.
5. Diagnose common baud-rate and COM-port issues.

## Session Plan

| Time | Activity | Details |
|---|---|---|
| 13:00-13:30 | LPUART on MCX A | Baud-rate generation, clock source, FIFOs, low-power UART |
| 13:30-14:15 | Hello World UART | `LPUART_GetDefaultConfig`, `LPUART_Init`, `LPUART_WriteBlocking` |
| 14:15-15:00 | Lab + AI session | UART echo and LED menu controlled from terminal |

## AI Assistant Prompts

### Prompt: LPUART0 Init

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK.
LPUART0 is connected to MCU-Link VCOM over USB. Configure it for 115200 8N1 using polling, no DMA and no IRQ.
Use MCUXpresso SDK APIs: CLOCK_SetClockDiv or the correct clock divider API for this SDK, CLOCK_AttachClk, LPUART_GetDefaultConfig, LPUART_Init, LPUART_WriteBlocking.
Show how to retarget printf and explain how srcClock_Hz affects baud-rate calculation.
```

### Prompt: Debug Baud-Rate Error

```text
FRDM-MCXA153, LPUART0, MCUXpresso SDK.
Problem: LPUART_Init returns kStatus_LPUART_BaudrateNotSupport.
baudRate_Bps=115200, srcClock_Hz=48000000UL.
I attached a clock to LPUART0.
What should I verify about the clock divider, attached source, and the srcClock_Hz parameter passed to LPUART_Init?
```

## Configure LPUART0 in MCUXpresso Config Tools

### Route UART Pins

In the **Pins** view, select the LPUART0 RX/TX signals.

![Select LPUART signal](../img/lab0/setup_pinout_uart.png)

![Assign LPUART0 signal](../img/lab0/configurator_uart_selection.png)

Open the LPUART0 routing dialog and enable RX/TX.

![Route LPUART0 RX and TX](../img/lab0/configurator_uart_pins.png)

### Fix SysTick Configuration Errors

When clicking **Update Code**, Config Tools may report a SysTick clock-source error if the `.mex` file uses a clock source not available on MCXA153.

![Update Files - SysTick error](../img/lab0/debug_config_error.png)

Use the AI assistant to inspect the `.mex` error if needed.

![AI explains SysTick .mex error](../img/lab0/debug_config_error_output.png)

The Config Tools detail view shows the problematic field.

![Config Tools - SysTick clock-source error](../img/lab0/debug_config_error_trace.png)

Set the SysTick clock source to a valid system clock. The calculated interrupt period should update.

![SysTick fixed with valid clock source](../img/lab0/debug_config_error_fixed.png)

## Serial Monitor

Install the **Serial Monitor** extension in VS Code.

![Install Serial Monitor extension](../img/lab-uart/install_serial_monitor.png)

Open the Serial Monitor and select the MCU-Link VCOM port, for example `COM3 - MCU-Link VCom Port`.

![Open Serial Monitor and select COM port](../img/lab0/open_serial_monitor_port.png)

Expected output:

```text
Hello from FRDM-MCXA153!
```

![Serial Monitor output](../img/lab0/open_serial_monitor_port_output.png)

## Suggested Exercise

Implement a simple UART menu:

```text
1 - turn red LED on
2 - turn green LED on
3 - turn blue LED on
0 - turn all LEDs off
? - print menu
```

Use polling first. Keep the code simple and verify every received character through the serial terminal.

## Common Pitfalls

- The source clock passed to `LPUART_Init()` must match the real configured peripheral clock.
- Clock divider setup must happen before UART initialization.
- Windows shows the MCU-Link VCOM port only when the proper driver is installed.
- Blocking UART writes can delay time-sensitive logic in later labs.
- Do not trust AI-generated clock names blindly; verify against the SDK headers and generated clock files.

## Deliverable

Submit:

1. a UART Hello World project;
2. an LED menu controlled from the serial terminal;
3. a note showing the UART source clock and baud-rate settings;
4. one screenshot of Serial Monitor output.

---

[<- L1: GPIO - Digital Control and Buttons](../l1-gpio) - [L3: External Interrupts and NVIC ->](../l3-intreruperi)

