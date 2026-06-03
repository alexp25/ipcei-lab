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

#define PWM_FREQUENCY_HZ 1000U
#define FADE_STEP_DELAY_US 10000U
#define MIN_DUTY_PERCENT 0U
#define MAX_DUTY_PERCENT 100U
#define FADE_STEPS (MAX_DUTY_PERCENT - MIN_DUTY_PERCENT)
#define DUTY_PRINT_INTERVAL 25U

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

static void PWM_InitLedChannel(pwm_submodule_t module, uint8_t initialDutyPercent)
{
    pwm_signal_param_t pwmSignal;

    pwmSignal.pwmChannel       = kPWM_PwmX;
    pwmSignal.level            = kPWM_LowTrue;
    pwmSignal.dutyCyclePercent = initialDutyPercent;
    pwmSignal.deadtimeValue    = 0U;
    pwmSignal.faultState       = kPWM_PwmFaultState0;
    pwmSignal.pwmchannelenable = true;

    status_t status = PWM_SetupPwm(FLEXPWM0,
                                    module,
                                    &pwmSignal,
                                    1U,
                                    kPWM_EdgeAligned,
                                    PWM_FREQUENCY_HZ,
                                    SystemCoreClock);
    if (status != kStatus_Success)
    {
        PRINTF("PWM setup failed for module %u: %d\r\n", module, status);
    }

    /*
     * The FRDM-MCXA153 lab does not use FlexPWM fault inputs.
     * Clear the fault disable map, otherwise a mapped fault can keep PWMX off.
     */
    PWM_SetupFaultDisableMap(FLEXPWM0, module, kPWM_PwmX, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0, kPWM_PwmX, module);
}

static void FlexPWM0_InitLedPwm(void)
{
    pwm_config_t pwmConfig;

    PWM_GetDefaultConfig(&pwmConfig);

    PWM_Init(FLEXPWM0, kPWM_Module_0, &pwmConfig); /* PWM0_X0: red LED */
    PWM_Init(FLEXPWM0, kPWM_Module_1, &pwmConfig); /* PWM0_X1: green LED */

    PWM_InitLedChannel(kPWM_Module_0, MIN_DUTY_PERCENT);
    PWM_InitLedChannel(kPWM_Module_1, MAX_DUTY_PERCENT);

    PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
    PWM_StartTimer(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1);
}

static void RGB_SetRedGreenDuty(uint8_t redDutyPercent, uint8_t greenDutyPercent)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_EdgeAligned, redDutyPercent);
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_EdgeAligned, greenDutyPercent);
    PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();
    BOARD_InitDebugConsole();
    FlexPWM0_InitLedPwm();
    PRINTF("FlexPWM LED fade started\r\n");

    uint16_t phase = 0U;

    while (1)
    {
        uint8_t red_duty, green_duty;

        if (phase < FADE_STEPS)
        {
            red_duty   = (uint8_t)(MIN_DUTY_PERCENT + phase);
            green_duty = (uint8_t)(MAX_DUTY_PERCENT - phase);
        }
        else
        {
            red_duty   = (uint8_t)(MAX_DUTY_PERCENT - (phase - FADE_STEPS));
            green_duty = (uint8_t)(MIN_DUTY_PERCENT + (phase - FADE_STEPS));
        }

        RGB_SetRedGreenDuty(red_duty, green_duty);

        if ((phase % DUTY_PRINT_INTERVAL) == 0U)
        {
            PRINTF("duty: red=%u green=%u\r\n", red_duty, green_duty);
        }

        if (++phase >= (2U * FADE_STEPS))
        {
            phase = 0U;
        }

        SDK_DelayAtLeastUs(FADE_STEP_DELAY_US, SystemCoreClock);
    }
}
