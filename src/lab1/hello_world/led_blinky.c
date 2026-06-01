/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include <stdio.h>

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint8_t flag = 0;
char messageBuffer[100];
uint32_t counter = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    /* Toggle pin connected to LED */
    // GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
    flag = 1;
}

void init_pins()
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO,
                  BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO,
                  BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);

    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON);
    SDK_DelayAtLeastUs(500000, SystemCoreClock);
    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);

    GPIO_PinWrite(BOARD_LED_GREEN_GPIO,
                  BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_ON);
    SDK_DelayAtLeastUs(500000, SystemCoreClock);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO,
                  BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF);

    GPIO_PinWrite(BOARD_LED_BLUE_GPIO,
                  BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_ON);
    SDK_DelayAtLeastUs(500000, SystemCoreClock);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO,
                  BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_InitHardware();

    BOARD_InitDebugConsole();

    PRINTF("Hello from FRDM-MCXA153!\r\n");

    init_pins();

    SysTick_Config(SystemCoreClock / 2U);

    while (1)
    {
        if (flag)
        {
            flag = 0;
            sprintf(messageBuffer, "Hello from FRDM-MCXA153! (%ld)\r\n", counter);
            counter += 1;
            PRINTF("%s", messageBuffer);
        }
    }
}
