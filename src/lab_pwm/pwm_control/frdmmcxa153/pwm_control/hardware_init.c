/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockFRO12M();
    BOARD_InitBootPeripherals();

    LED_BLUE_INIT(LOGIC_LED_OFF); /* Starter: red and green use PWM, blue is left as GPIO. */
}
/*${function:end}*/
