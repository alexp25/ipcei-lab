/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "peripherals.h"
#include <stdbool.h>

#include "app/uart.h"

#include "fsl_debug_console.h"
#include "app/ili9341_demo.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC_SW2_CHANNEL 14U
#define ADC_COMMAND_ID 1U
#define ADC_TRIGGER_ID 0U
#define ADC_PRINT_TICKS 100U
#define PWM_FADE_DUTY_MAX 100U
#define P3T1755_I2C_ADDRESS 0x48U
#define P3T1755_TEMP_REG 0x00U
#define P3T1755_TEMP_RX_SIZE 2U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void PWM0_LED_InitOutput(void);
static void PWM0_LED_UpdateFade(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint16_t counterPrintTick = 0;
static volatile uint32_t printCounter = 0;
static volatile bool flag = false;
static volatile uint8_t pwmDutyCycle = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    counterPrintTick++;
    if (counterPrintTick >= ADC_PRINT_TICKS)
    {
        counterPrintTick = 0;
        flag = true;
        GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
    }

    PWM0_LED_UpdateFade();
}


static void PWM0_LED_InitOutput(void)
{
    PWM_SetupFaultDisableMap(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0_A, FLEXPWM0_SM0);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
}

static void PWM0_LED_UpdateFade(void)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_SignedCenterAligned,
                           pwmDutyCycle);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);

    pwmDutyCycle++;
    if (pwmDutyCycle >= PWM_FADE_DUTY_MAX)
    {
        pwmDutyCycle = 0;
    }
}


/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_InitHardware();
    PWM0_LED_InitOutput();

    status_t status = ILI9341_DemoInit();
    if (status != kStatus_Success)
    {
        PRINTF("ILI9341 demo init failed: %ld\r\n", (long)status);
        while (1)
        {
        }
    }

    uint32_t frame = 0U;

    while (1)
    {
        uint16_t screensaver_value =
            (uint16_t)((((frame * 29U) & 0x0FFFU) + (((frame * 113U) + 900U) & 0x0FFFU)) / 2U);
        ILI9341_DemoUpdate(screensaver_value);
        frame++;
        SDK_DelayAtLeastUs(2000U, CLOCK_GetCoreSysClkFreq());
    }
}
