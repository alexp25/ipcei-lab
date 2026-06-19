# LCD Keypad Shield

This demo drives an Arduino-style HD44780 16x2 LCD Keypad Shield from the
FRDM-MCXA153 Arduino-compatible headers. The LCD uses 4-bit mode and the five
buttons are read from the resistor ladder on Arduino A0.

## Pin mapping

| Shield signal | Arduino pin | MCXA153 pin |
| --- | --- | --- |
| LCD RS | D8 | P2_7 |
| LCD EN | D9 | P3_31 |
| LCD D4 | D4 | P2_5 |
| LCD D5 | D5 | P3_12 |
| LCD D6 | D6 | P3_13 |
| LCD D7 | D7 | P3_1 |
| Backlight | D10 | P3_11 |
| Keypad | A0 | P1_10 / ADC0_A8 |

P3_12 and P3_13 also drive the board's red and green LEDs, so those LEDs may
flicker while LCD data is transferred.

## Voltage compatibility

FRDM-MCXA153 GPIO and ADC operate at 3.3 V. Do not allow a 5 V shield signal to
drive an MCU pin. In particular, many LCD Keypad Shields power the A0 resistor
ladder from 5 V; use a divider or level shifter that limits A0 to 3.3 V, or run
a verified 3.3 V-compatible shield from 3.3 V. A divider also changes the ADC
button values, so adjust `thresholds_10bit` if necessary.

The LCD interface is write-only, which prevents the 5 V LCD data bus from
driving the MCU. However, a 5 V-powered HD44780-compatible controller may not
guarantee that 3.3 V is recognized as a logic high. Use a 3.3 V-compatible LCD
module or level-shift D4-D9 when required by the module data sheet.

D10 backlight control is enabled and active-high for the tested shield. Common
shield revisions use different transistor polarity and some revisions can
stress the host pin. Check the shield schematic; set
`LCD_SHIELD_ENABLE_BACKLIGHT_CONTROL` to `0` to disconnect software control, or
change `LCD_SHIELD_BACKLIGHT_ACTIVE_HIGH` for an active-low revision.

## Configuration

LCD, keypad, and FRDM board mapping are separate modules.

The default button thresholds are Arduino 10-bit values:

- RIGHT: 0-80
- UP: 81-200
- DOWN: 201-400
- LEFT: 401-600
- SELECT: 601-800
- NONE: above 800

The keypad driver scales the board's 12-bit ADC result to this 10-bit range and
requires three consecutive matching samples before changing the reported key.
ADC conversion polling has a timeout, so a missing shield cannot block forever.
The FRDM adapter supplies a board-specific threshold profile because a
5 V-powered ladder, after being limited safely to the 3.3 V ADC range, does not
match the generic Arduino values. The serial monitor prints raw and scaled ADC
readings every 250 ms; use those readings to tune `s_keypadThresholds` for the
actual shield and divider tolerances.

## Running

Build and flash the debug target, then open the debug UART at 115200 8-N-1.
The LCD displays `FRDM-MCXA153` and updates the second line with the stable
button name. If the backlight is visible but characters are not, adjust the
shield's contrast potentiometer before changing software.
