# SPFD5408 TFT Shield

This demo drives an Arduino-style 2.4-inch TFT shield that follows the
MCUFRIEND/SPFD5408 parallel-bus pinout. The display uses the classic Adafruit/MCUFRIEND 8-bit 8080-style bus: DB0/DB1 on Arduino D8/D9, DB2-DB7 on D2-D7, and control signals on A0-A3.

The linked `JoaoLopesF/SPFD5408` Arduino library is archived and specific to
cheap undocumented shields. This project does not import the Arduino library
directly; it provides a small MCUXpresso-native, write-only driver and uses the
same hard-coded `0x9341`/SPFD5408-compatible startup approach recommended by
that library.

## Pin mapping

| Shield signal | Arduino pin | MCXA153 pin |
| --- | ---: | --- |
| DB0 | D8 | P2_7 |
| DB1 | D9 | P3_31 |
| DB2 | D2 | P2_4 |
| DB3 | D3 | P3_0 |
| DB4 | D4 | P2_5 |
| DB5 | D5 | P3_12 |
| DB6 | D6 | P3_13 |
| DB7 | D7 | P3_1 |
| RD | A0 | P1_10 |
| WR | A1 | P1_12 |
| CD/RS | A2 | P1_13 |
| CS | A3 | P2_0 |
| RESET | A4 | P3_31, conflicts with D9/DB1 on FRDM-MCXA153 |

D3, D5, and D6 are shared with the board RGB LEDs. The board LED initialization is disabled so the TFT owns those pins. Arduino D9 and A4 are both P3_31 on FRDM-MCXA153; this conflicts with shields that use D9 as DB1 and A4 as TFT reset. Use software reset only, or isolate the shield A4 reset pin.

## Electrical notes

FRDM-MCXA153 GPIO is 3.3 V. Many Arduino TFT shields are 5 V Uno-era designs.
Use only shields that are 3.3 V logic compatible, or add level shifting on
D0-D7 and A0-A4. The current driver is write-only and keeps `RD` high so the
shield should not drive the MCU data bus during normal operation.

Touch and SD-card support are not enabled in this first bring-up. Touch shares
some TFT bus pins and needs an additional safe analog-read strategy.

## Running

Build and flash the debug target, then open the debug UART at 115200 8-N-1. The
demo repeatedly fills the display with color bars and large rectangles. If the
screen stays white, verify the controller is actually SPFD5408/ILI9341-like and
check whether the shield requires 5 V logic levels.

