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

    /*
     * The Arduino shields use D3, D5, and D6, which are shared with the board
     * RGB LEDs. Leave those pins owned by the active shield driver.
     */
}
/*${function:end}*/
