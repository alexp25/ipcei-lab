# Arduino Multi-Function Shield demo

## Overview
This demo drives an Arduino Multi-Function Shield on the FRDM-MCXA153 Arduino-compatible headers.
It refreshes the four-digit 7-segment display through cascaded 74HC595 shift registers, toggles the
shield LEDs, debounces S1/S2/S3, reads the A0 potentiometer, and beeps briefly when a button is pressed.

## Supported Boards
- [FRDM-MCXA153](#board-specific-information-for-frdmmcxa153)

---

## Board-Specific Information for frdmmcxa153

### Hardware requirements
- Type-C USB cable
- FRDM-MCXA153 board
- Arduino Multi-Function Shield, Hackatronics/Cohesive Computing compatible pinout
- Personal Computer

### Board settings
The shield is Arduino-style 5 V hardware. Use 3.3 V-safe operation or level shifting for MCU inputs,
and keep the A0 potentiometer voltage inside the ADC reference range.

### Prepare the Demo
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J15) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

### Running the demo
When the demo runs, the 7-segment display shows the scaled A0 potentiometer reading, the LEDs chase,
and pressing S1, S2, or S3 prints an event and enables a short buzzer beep.
