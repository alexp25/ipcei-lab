#include "app.h"
#include "board.h"
#include "frdmmcxa153_lcd_shield.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "hd44780.h"
#include "lcd_demo_config.h"
#include "lcd_keypad.h"

#include <stdint.h>

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

int main(void)
{
    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    frdmmcxa153_lcd_shield_t shield;
    hd44780_t lcd;
    lcd_keypad_t keypad;
    lcd_keypad_button_t button = kLcdKeypadButtonNone;
    lcd_keypad_button_t displayed_button = (lcd_keypad_button_t)-1;
#if LCD_KEYPAD_DIAGNOSTIC_LOG_ENABLE
    uint32_t diagnostic_elapsed_us = 0U;
#endif

    PRINTF("FRDM-MCXA153 LCD keypad shield demo\r\n");

    if (!FRDMMCXA153_LcdShieldInit(&shield) ||
        !HD44780_Init(&lcd, &shield.lcd) ||
        !LCD_KeypadInit(&keypad, &shield.keypad))
    {
        PRINTF("LCD keypad shield initialization failed\r\n");
        while (1)
        {
            SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
        }
    }

    HD44780_WriteLine(&lcd, 0U, "FRDM-MCXA153");
    HD44780_WriteLine(&lcd, 1U, "Key: NONE");
    PRINTF("LCD initialized; adjust the shield contrast potentiometer if the backlight is on but text is not visible.\r\n");

    while (1)
    {
        if (LCD_KeypadPoll(&keypad, &button))
        {
            if (button != displayed_button)
            {
                char line[17] = "Key: ";
                const char *name = LCD_KeypadButtonName(button);
                uint8_t index = 5U;

                while ((*name != '\0') && (index < (sizeof(line) - 1U)))
                {
                    line[index++] = *name++;
                }
                line[index] = '\0';

                HD44780_WriteLine(&lcd, 1U, line);
                PRINTF("Button: %s\r\n", LCD_KeypadButtonName(button));
                displayed_button = button;
            }
#if LCD_KEYPAD_DIAGNOSTIC_LOG_ENABLE
            diagnostic_elapsed_us += LCD_KEYPAD_POLL_PERIOD_US;
            if (diagnostic_elapsed_us >= LCD_KEYPAD_DIAGNOSTIC_LOG_PERIOD_US)
            {
                PRINTF("ADC raw=%u, 10-bit=%u, key=%s\r\n",
                       LCD_KeypadGetLastRawValue(&keypad),
                       LCD_KeypadGetLast10BitValue(&keypad),
                       LCD_KeypadButtonName(button));
                diagnostic_elapsed_us = 0U;
            }
#endif
        }
        SDK_DelayAtLeastUs(LCD_KEYPAD_POLL_PERIOD_US, SystemCoreClock);
    }
}
