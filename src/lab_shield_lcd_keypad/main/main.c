#include "app.h"
#include "board.h"
#include "dfrobot_lcd_shield.h"
#include "dfrobot_lcd_shield_keypad.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include <stdint.h>

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
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

static void ShowButton(dfrobot_rgb_lcd_t *lcd, dfrobot_lcd_shield_button_t button)
{
    char line[17] = "Key: ";
    const char *name = DFRobot_LcdShieldKeypadButtonName(button);
    uint8_t index = 5U;

    while ((*name != '\0') && (index < (sizeof(line) - 1U)))
    {
        line[index++] = *name++;
    }
    line[index] = '\0';

    DFRobot_RgbLcd_WriteLine(lcd, 1U, line);
}

int main(void)
{
    dfrobot_lcd_shield_t shield;
    dfrobot_lcd_shield_keypad_t keypad;
    dfrobot_lcd_shield_button_t button = kDFRobotLcdShieldButtonNone;
    dfrobot_lcd_shield_button_t displayedButton = (dfrobot_lcd_shield_button_t)-1;
    uint32_t diagnosticTicks = 0U;
    int colorR = 255;
    int colorG = 0;
    int colorB = 0;
    int stage = 0;

    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    PRINTF("FRDM-MCXA153 DFRobot RGBLCD1602 I2C demo\r\n");
    PRINTF("Using LPI2C0 on Arduino header SDA=P1_8 and SCL=P1_9\r\n");
    PRINTF("LCD address 0x%02X, RGB address 0x%02X\r\n",
           DFROBOT_RGB_LCD_LCD_ADDRESS,
           DFROBOT_LCD_SHIELD_RGB_ADDRESS);

    DFRobot_LcdShieldBusInit();
    PRINTF("I2C scan:");
    for (uint8_t address = 0x08U; address <= 0x77U; ++address)
    {
        if (DFRobot_LcdShieldProbeAddress(address))
        {
            PRINTF(" 0x%02X", address);
        }
    }
    PRINTF("\r\n");

    if (!DFRobot_LcdShieldKeypadInit(&keypad))
    {
        PRINTF("Keypad ADC initialization failed\r\n");
    }

    if (!DFRobot_LcdShieldInit(&shield))
    {
        PRINTF("RGBLCD1602 initialization reported an I2C write failure; continuing to show scan diagnostics.\r\n");
    }

    DFRobot_RgbLcd_SetRGB(&shield.lcd, (uint8_t)colorR, (uint8_t)colorG, (uint8_t)colorB);
    DFRobot_RgbLcd_WriteString(&shield.lcd, "hello, world!");
    ShowButton(&shield.lcd, kDFRobotLcdShieldButtonNone);
    PRINTF("RGBLCD1602 init sequence sent\r\n");

    while (1)
    {
        UpdateColor(&colorR, &colorG, &colorB, &stage);

        if (DFRobot_LcdShieldKeypadPoll(&keypad, &button))
        {
            ++diagnosticTicks;
            if (diagnosticTicks >= 5U)
            {
                PRINTF("ADC raw=%u key=%s\r\n",
                       DFRobot_LcdShieldKeypadGetLastRawValue(&keypad),
                       DFRobot_LcdShieldKeypadButtonName(button));
                diagnosticTicks = 0U;
            }

            if (button != displayedButton)
            {
                ShowButton(&shield.lcd, button);
                PRINTF("Button: %s raw=%u\r\n",
                       DFRobot_LcdShieldKeypadButtonName(button),
                       DFRobot_LcdShieldKeypadGetLastRawValue(&keypad));
                displayedButton = button;
            }
        }

        DFRobot_RgbLcd_SetRGB(&shield.lcd, (uint8_t)colorR, (uint8_t)colorG, (uint8_t)colorB);
        SDK_DelayAtLeastUs(100000U, SystemCoreClock);
    }
}
