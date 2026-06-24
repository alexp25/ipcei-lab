# FRDM-MCXA153 DFRobot RGBLCD1602 I2C Demo

This lab targets the DFRobot RGBLCD1602 I2C module/library protocol used by the Arduino `DFRobot_RgbLcd` examples.

## Hardware Mapping

- LCD controller I2C address: `0x3E`
- RGB controller I2C address: `0x60` by default
- FRDM-MCXA153 Arduino header SDA: `P1_8`, package pin 2
- FRDM-MCXA153 Arduino header SCL: `P1_9`, package pin 3

The firmware uses LPI2C0 at 100 kHz on the Arduino R3 SDA/SCL pins.

## Electrical Note

The FRDM-MCXA153 I/O domain is 3.3 V. Make sure the LCD module I2C pull-ups are to 3.3 V, or use level shifting if the module is powered/pulled up to 5 V.

## Demo

The demo mirrors the working Arduino sketch: initialize the LCD, set RGB to red, print `hello, world!`, update the second line with elapsed seconds, and fade the RGB backlight. The serial monitor also prints an I2C scan so `0x3E` and `0x60` can be verified.

