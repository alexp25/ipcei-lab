/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "app/dfrobot_lcd_shield.h"
#include "app/dfrobot_lcd_shield_keypad.h"
#include "peripherals.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"

#include <stdbool.h>
#include <stdint.h>

#define LED_TOGGLE_TICKS 100U
#define PWM_FADE_DUTY_MAX 100U
#define LCD_KEY_REFRESH_TICKS 5U

static volatile uint16_t s_ledToggleTicks = 0U;
static volatile uint8_t s_pwmDutyCycle = 0U;

static void PWM0_LED_InitOutput(void);
static void PWM0_LED_UpdateFade(void);

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

void SysTick_Handler(void)
{
    s_ledToggleTicks++;
    if (s_ledToggleTicks >= LED_TOGGLE_TICKS)
    {
        s_ledToggleTicks = 0U;
        GPIO_PortToggle(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
    }

    PWM0_LED_UpdateFade();
}

static void PWM0_LED_UpdateFade(void)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0_PERIPHERAL,
                           FLEXPWM0_SM0,
                           FLEXPWM0_SM0_A,
                           kPWM_SignedCenterAligned,
                           s_pwmDutyCycle);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);

    s_pwmDutyCycle++;
    if (s_pwmDutyCycle >= PWM_FADE_DUTY_MAX)
    {
        s_pwmDutyCycle = 0U;
    }
}

static void PWM0_LED_InitOutput(void)
{
    PWM_SetupFaultDisableMap(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0_A, FLEXPWM0_SM0);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
}

static void UpdateColor(int *colorR, int *colorG, int *colorB, int *stage)
{
    const int speed = 5;

    switch (*stage)
    {
        case 0:
            *colorG += speed;
            if (*colorG >= 255)
            {
                *colorG = 255;
                *stage = 1;
            }
            break;
        case 1:
            *colorR -= speed;
            if (*colorR <= 0)
            {
                *colorR = 0;
                *stage = 2;
            }
            break;
        case 2:
            *colorB += speed;
            if (*colorB >= 255)
            {
                *colorB = 255;
                *stage = 3;
            }
            break;
        case 3:
            *colorG -= speed;
            if (*colorG <= 0)
            {
                *colorG = 0;
                *stage = 4;
            }
            break;
        case 4:
            *colorR += speed;
            if (*colorR >= 255)
            {
                *colorR = 255;
                *stage = 5;
            }
            break;
        default:
            *colorB -= speed;
            if (*colorB <= 0)
            {
                *colorB = 0;
                *stage = 0;
            }
            break;
    }
}

static bool ShowButton(dfrobot_rgb_lcd_t *lcd, dfrobot_lcd_shield_button_t button)
{
    char line[17] = "Key: ";
    const char *name = DFRobot_LcdShieldKeypadButtonName(button);
    uint8_t index = 5U;

    while ((*name != '\0') && (index < (sizeof(line) - 1U)))
    {
        line[index++] = *name++;
    }
    line[index] = '\0';

    return DFRobot_RgbLcd_WriteLine(lcd, 1U, line);
}

/*!
 * @brief Main function
 */
int main(void)
{
    dfrobot_lcd_shield_t shield;
    dfrobot_lcd_shield_keypad_t keypad;
    dfrobot_lcd_shield_button_t button = kDFRobotLcdShieldButtonNone;
    dfrobot_lcd_shield_button_t displayedButton = (dfrobot_lcd_shield_button_t)-1;
    uint32_t diagnosticTicks = 0U;
    uint32_t lcdKeyRefreshTicks = 0U;
    int colorR = 255;
    int colorG = 0;
    int colorB = 0;
    int stage = 0;

    BOARD_InitHardware();
    BOARD_InitDebugConsole();
    PWM0_LED_InitOutput();

    PRINTF("FRDM-MCXA153 DFRobot RGBLCD1602 I2C demo\r\n");
    PRINTF("Using configured LPI2C0 pins. Check pin_mux.c for the active SDA/SCL pads.\r\n");
    PRINTF("Expected LCD address 0x%02X, default RGB address 0x%02X\r\n",
           DFROBOT_RGB_LCD_LCD_ADDRESS,
           DFROBOT_LCD_SHIELD_RGB_ADDRESS);

    DFRobot_LcdShieldBusInit();
    bool lcdFound = DFRobot_LcdShieldProbeAddress(DFROBOT_RGB_LCD_LCD_ADDRESS);
    PRINTF("LCD probe 0x%02X: %s\r\n", DFROBOT_RGB_LCD_LCD_ADDRESS, lcdFound ? "OK" : "missing");
    if (!lcdFound)
    {
        PRINTF("LCD controller was not found at 0x%02X; verify wiring against the configured LPI2C0 pins.\r\n",
               DFROBOT_RGB_LCD_LCD_ADDRESS);
    }

    if (!DFRobot_LcdShieldInit(&shield))
    {
        PRINTF("RGBLCD1602 initialization reported an I2C write failure; continuing to show scan diagnostics.\r\n");
    }
    PRINTF("Selected RGB address 0x%02X\r\n", shield.lcd.config.rgbAddress);

    DFRobot_RgbLcd_SetRGB(&shield.lcd, (uint8_t)colorR, (uint8_t)colorG, (uint8_t)colorB);
    DFRobot_RgbLcd_WriteLine(&shield.lcd, 0U, "hello, world!");
    if (ShowButton(&shield.lcd, kDFRobotLcdShieldButtonNone))
    {
        displayedButton = kDFRobotLcdShieldButtonNone;
    }
    PRINTF("RGBLCD1602 init sequence sent\r\n");

    if (!DFRobot_LcdShieldKeypadInit(&keypad))
    {
        PRINTF("Keypad ADC initialization failed\r\n");
    }

    while (1)
    {
        UpdateColor(&colorR, &colorG, &colorB, &stage);

        if (DFRobot_LcdShieldKeypadPoll(&keypad, &button))
        {
            bool refreshLcdKey = (button != displayedButton);

            ++diagnosticTicks;
            if (diagnosticTicks >= 5U)
            {
                PRINTF("ADC raw=%u key=%s\r\n",
                       DFRobot_LcdShieldKeypadGetLastRawValue(&keypad),
                       DFRobot_LcdShieldKeypadButtonName(button));
                diagnosticTicks = 0U;
            }

            ++lcdKeyRefreshTicks;
            if (lcdKeyRefreshTicks >= LCD_KEY_REFRESH_TICKS)
            {
                refreshLcdKey = true;
                lcdKeyRefreshTicks = 0U;
            }

            if (refreshLcdKey)
            {
                if (ShowButton(&shield.lcd, button))
                {
                    if (button != displayedButton)
                    {
                        PRINTF("Button: %s raw=%u\r\n",
                               DFRobot_LcdShieldKeypadButtonName(button),
                               DFRobot_LcdShieldKeypadGetLastRawValue(&keypad));
                    }
                    displayedButton = button;
                }
                else
                {
                    PRINTF("LCD key line update failed\r\n");
                }
            }
        }
        else
        {
            ++diagnosticTicks;
            if (diagnosticTicks >= 5U)
            {
                PRINTF("Keypad ADC poll timeout\r\n");
                diagnosticTicks = 0U;
            }
        }

        DFRobot_RgbLcd_SetRGB(&shield.lcd, (uint8_t)colorR, (uint8_t)colorG, (uint8_t)colorB);
        SDK_DelayAtLeastUs(100000U, SystemCoreClock);
    }
}
