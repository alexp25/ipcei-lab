/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_pwm.h"
#include "peripherals.h"

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

static void EnablePwmOutput_BlueLED(void)
{
    PWM_SetupFaultDisableMap(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0_A, FLEXPWM0_SM0);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
}

static void SetDutyCycle_BlueLED(uint8_t dutyPercent)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_SignedCenterAligned, dutyPercent);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
}

static uint8_t Blue_GetFadeDuty(uint16_t phase)
{
    uint8_t step = (uint8_t)(phase % COLOR_FADE_STEPS);

    if (phase < COLOR_FADE_STEPS)
    {
        return (uint8_t)(MIN_DUTY_PERCENT + step);
    }

    return (uint8_t)(MAX_DUTY_PERCENT - step);
}

uint8_t pwm_blue_led = 0;

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();
    BOARD_InitDebugConsole();
    Blue_EnablePwmOutput();
    
    PRINTF("application started\r\n");

    while (1)
    {        

        SetDutyCycle_BlueLED(pwm_blue_led);
        pwm_blue_led += 1;
       
        if (pwm_blue_led >= 255)
        {
            pwm_blue_led = 0U;
        }

        SDK_DelayAtLeastUs(10000, SystemCoreClock);
    }
}
