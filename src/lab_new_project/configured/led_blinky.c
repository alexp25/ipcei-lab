/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "peripherals.h"
#include <stddef.h>

#include "app/uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PWM_FADE_DUTY_MIN 0
#define PWM_FADE_DUTY_MAX 100

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t PWMDutyCycle = 0;
volatile uint16_t timerCounter = 0;
volatile uint16_t printCounter = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    if (timerCounter >= 100)
    {
        timerCounter = 0;
        GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);

        LPUART0_WriteString("counter=");
        LPUART0_WriteU32(printCounter);
        LPUART0_WriteString("\r\n");

        printCounter++;
    }
    timerCounter++;

    PWM_UpdatePwmDutycycle(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_SignedCenterAligned,
        PWMDutyCycle);

    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);

    if (PWMDutyCycle >= 100)
    {
        PWMDutyCycle = 0;
    }
    else
    {
        PWMDutyCycle++;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_InitHardware();
    PWM_SetupFaultDisableMap(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0_A, FLEXPWM0_SM0);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);

    while (1)
    {
    }
}
