---
title: "L3 - External Interrupts and NVIC"
description: "GPIO polling, debounce, and conversion to GPIO interrupts"
nav_order: 4
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_interrupts/main
---

# L3 - External Interrupts and NVIC

**From SW3 polling to GPIO/NVIC interrupts**

---

| | |
|---|---|
| **Session** | GPIO + NVIC |
| **Peripheral** | `GPIO`, `GPIO IRQ`, `NVIC` |
| **Hardware** | FRDM-MCXA153, SW3 on `GPIO1` pin `7` / `P1_7`, RGB LED |
| **Project** | `src/lab_interrupts/main` |

## Context and Motivation

Before using interrupts, the project starts from a simple polling implementation. The program periodically reads SW3 and toggles the RGB LEDs when it detects a new press.

The starter code separates three useful ideas:

- `check_button_pressed()` reads the button without debounce.
- `check_button_pressed_debounce()` reads the button with software debounce.
- `on_button_pressed()` contains the shared action: toggle LEDs and print a message.

The goal is to convert button detection from polling to a GPIO interrupt handled through the NVIC.

## Objectives

By the end of the lab you should be able to:

1. Build and run `src/lab_interrupts/main`.
2. Observe the difference between polling without debounce and polling with debounce.
3. Explain why mechanical buttons bounce.
4. Configure SW3 as a GPIO interrupt on falling edge.
5. Enable the correct NVIC IRQ.
6. Clear the interrupt flag in the ISR.
7. Update the `.mex` file so the SW3/IRQ configuration is visible in Config Tools.

## Exercises

### Exercise 0 - Compile and Run

```powershell
cd src/lab_interrupts/main
cmake --preset debug
cmake --build --preset debug
```

Flash the board, open Serial Monitor at `115200`, press SW3, and verify messages such as:

```text
SW3 pressed: toggled LEDs (0)
SW3 pressed: toggled LEDs (1)
```

### Exercise 1 - Observe Button Bounce

Start with the non-debounced function enabled:

```c
while (1)
{
    check_button_pressed();

    // check_button_pressed_debounce();
}
```

Press SW3 once and count how many messages appear. Then switch to the debounced version:

```c
while (1)
{
    // check_button_pressed();

    check_button_pressed_debounce();
}
```

Expected result: with debounce, one physical press should produce one message and one LED toggle.

### Exercise 2 - Convert SW3 to Interrupts

Minimum requirements:

1. Configure SW3 as GPIO input on `GPIO1`, pin `7` (`P1_7`).
2. Enable interrupt on falling edge.
3. Enable `GPIO1_IRQn`.
4. Implement `GPIO1_IRQHandler` or the board macro `BOARD_SW3_IRQ_HANDLER`.
5. Check that the interrupt flag belongs to SW3.
6. Clear the interrupt flag before leaving the ISR.
7. Call `on_button_pressed()` for each valid press.
8. Remove polling calls from `while (1)`.

Example initialization:

```c
void init_sw3_interrupt(void)
{
    init_sw3_gpio();

    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO,
                               BOARD_SW3_GPIO_PIN,
                               kGPIO_InterruptFallingEdge);
    GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);

    NVIC_ClearPendingIRQ(BOARD_SW3_IRQ);
    EnableIRQ(BOARD_SW3_IRQ);
}
```

Example ISR:

```c
void BOARD_SW3_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
    {
        GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);
        on_button_pressed();
    }
}
```

The main loop can then wait for interrupts:

```c
init_pins();
init_sw3_interrupt();

while (1)
{
    __WFI();
}
```

For this lab, calling `on_button_pressed()` directly in the ISR is acceptable. In production firmware, ISRs should normally be short and should set a `volatile` flag that is processed in the main loop.

## Update the `.mex` File

After changing code, update the project configuration:

1. Open the active `.mex` file.
2. Confirm SW3 (`P1_7`) in the Pins tool.
3. Configure it as GPIO input with pull-up.
4. Configure falling-edge interrupt for SW3.
5. Save and regenerate generated files if needed.
6. Rebuild the project.

## AI Assistant Prompts

### Prompt: Convert Polling to SW3 Interrupt

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK.
Project: src/lab_interrupts/main.
SW3: GPIO1 pin 7 (P1_7), active-low, internal pull-up.
The current code uses polling and has:
- init_sw3_gpio();
- check_button_pressed();
- check_button_pressed_debounce();
- on_button_pressed();
Convert SW3 detection from polling to GPIO/NVIC interrupt.
Include falling-edge configuration, EnableIRQ(GPIO1_IRQn), ISR flag check, flag clear, and removal of polling from while(1).
Also explain what must be updated in the .mex file.
```

### Prompt: ISR Re-enters Continuously

```text
FRDM-MCXA153, SW3 on GPIO1 pin 7, falling-edge interrupt.
Problem: after the first press, GPIO1_IRQHandler is called continuously.
Check whether the interrupt flag for SW3 is cleared correctly.
Explain why an uncleared peripheral interrupt flag makes the NVIC re-enter the ISR immediately.
```

## Common Pitfalls

- SW3 is active-low: pressed means `GPIO_PinRead(...) == 0U`.
- Without debounce, one press can produce several messages.
- In interrupt mode, clear the GPIO interrupt flag; otherwise the ISR can run forever.
- After converting to interrupts, the main loop should not keep polling SW3.
- Verify the exact SW3 pin for this project: `GPIO1`, pin `7` (`P1_7`).
- Update `.mex`; do not leave the configuration only in handwritten code.
- AI may suggest APIs from another NXP family or Arduino. Verify against the SDK headers.

## Deliverable

Submit:

1. the polling version without debounce;
2. observations comparing no debounce vs debounce;
3. the interrupt-based version;
4. an updated `.mex` file showing SW3/IRQ configuration;
5. a short explanation of why the ISR must clear the interrupt flag.

---

[<- L2: LPUART - Serial Communication](../l2-lpuart) - [L4: Timer and PWM ->](../l4-timer-pwm)

