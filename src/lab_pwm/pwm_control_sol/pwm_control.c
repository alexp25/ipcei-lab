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
#define COLOR_FADE_STEPS 100U
#define COLOR_WHEEL_STEPS (3U * COLOR_FADE_STEPS)
#define DUTY_PRINT_INTERVAL 25U

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

static void PWM_InitLedChannel(pwm_submodule_t module, pwm_channels_t channel, uint8_t initialDutyPercent)
{
    pwm_signal_param_t pwmSignal;

    pwmSignal.pwmChannel       = channel;
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
     * Clear the fault disable map, otherwise a mapped fault can keep PWM off.
     */
    PWM_SetupFaultDisableMap(FLEXPWM0, module, channel, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0, channel, module);
}

static void FlexPWM0_InitLedPwm(void)
{
    pwm_config_t pwmConfig;

    PWM_GetDefaultConfig(&pwmConfig);

    PWM_Init(FLEXPWM0, kPWM_Module_0, &pwmConfig); /* PWM0_A0: blue, PWM0_X0: red */
    PWM_Init(FLEXPWM0, kPWM_Module_1, &pwmConfig); /* PWM0_X1: green LED */

    PWM_InitLedChannel(kPWM_Module_0, kPWM_PwmA, MIN_DUTY_PERCENT);  /* blue LED */
    PWM_InitLedChannel(kPWM_Module_0, kPWM_PwmX, MAX_DUTY_PERCENT);  /* red LED */
    PWM_InitLedChannel(kPWM_Module_1, kPWM_PwmX, MIN_DUTY_PERCENT);  /* green LED */

    PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
    PWM_StartTimer(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1);
}

static void RGB_SetDuty(uint8_t redDutyPercent, uint8_t greenDutyPercent, uint8_t blueDutyPercent)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_EdgeAligned, redDutyPercent);
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_0, kPWM_PwmA, kPWM_EdgeAligned, blueDutyPercent);
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_EdgeAligned, greenDutyPercent);
    PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
}

static void RGB_GetColorWheelDuty(uint16_t phase,
                                  uint8_t *redDutyPercent,
                                  uint8_t *greenDutyPercent,
                                  uint8_t *blueDutyPercent)
{
    uint8_t step = (uint8_t)(phase % COLOR_FADE_STEPS);

    if (phase < COLOR_FADE_STEPS)
    {
        *redDutyPercent   = (uint8_t)(MAX_DUTY_PERCENT - step);
        *greenDutyPercent = (uint8_t)(MIN_DUTY_PERCENT + step);
        *blueDutyPercent  = MIN_DUTY_PERCENT;
    }
    else if (phase < (2U * COLOR_FADE_STEPS))
    {
        *redDutyPercent   = MIN_DUTY_PERCENT;
        *greenDutyPercent = (uint8_t)(MAX_DUTY_PERCENT - step);
        *blueDutyPercent  = (uint8_t)(MIN_DUTY_PERCENT + step);
    }
    else
    {
        *redDutyPercent   = (uint8_t)(MIN_DUTY_PERCENT + step);
        *greenDutyPercent = MIN_DUTY_PERCENT;
        *blueDutyPercent  = (uint8_t)(MAX_DUTY_PERCENT - step);
    }
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
        uint8_t red_duty, green_duty, blue_duty;

        RGB_GetColorWheelDuty(phase, &red_duty, &green_duty, &blue_duty);
        RGB_SetDuty(red_duty, green_duty, blue_duty);

        if ((phase % DUTY_PRINT_INTERVAL) == 0U)
        {
            PRINTF("duty: red=%u green=%u blue=%u\r\n", red_duty, green_duty, blue_duty);
        }

        if (++phase >= COLOR_WHEEL_STEPS)
        {
            phase = 0U;
        }

        SDK_DelayAtLeastUs(FADE_STEP_DELAY_US, SystemCoreClock);
    }
}
