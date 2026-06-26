# USB HID Mouse Jiggler

## Overview
This application enumerates as a USB HID boot mouse and periodically sends small relative movement reports.

## Supported Boards
- [FRDM-MCXA153](#board-specific-information-for-frdmmcxa153)

---

## Board-Specific Information for frdmmcxa153

### Hardware requirements
- Type-C USB cable
- USB cable for the target USB connector
- FRDM-MCXA153 board
- Personal Computer

### Board settings
No special settings are required.

### Prepare the Demo
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port (J15) for flashing and debug.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Connect the host PC to the board's target USB device connector, not the MCU-Link/debug connector.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

### Running the demo
When the demo runs successfully, Windows enumerates a HID-compliant mouse and the pointer moves periodically.
