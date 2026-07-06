#include "dfrobot_rgb_lcd.h"

#include <string.h>

#define LCD_CLEARDISPLAY   (0x01U)
#define LCD_ENTRYMODESET   (0x04U)
#define LCD_DISPLAYCONTROL (0x08U)
#define LCD_FUNCTIONSET    (0x20U)
#define LCD_SETDDRAMADDR   (0x80U)

#define LCD_4BITMODE       (0x00U)
#define LCD_2LINE          (0x08U)
#define LCD_5X8DOTS        (0x00U)
#define LCD_DISPLAYON      (0x04U)
#define LCD_CURSOROFF      (0x00U)
#define LCD_BLINKOFF       (0x00U)
#define LCD_ENTRYLEFT      (0x02U)
#define LCD_ENTRYSHIFTDEC  (0x00U)

#define LCD_COMMAND_PREFIX (0x80U)
#define LCD_DATA_PREFIX    (0x40U)

#define RGB_ADDRESS_60_STYLE (0x60U)
#define RGB_ADDRESS_30_STYLE (0x30U)
#define RGB_ADDRESS_6B_STYLE (0x6BU)
#define RGB_ADDRESS_2D_STYLE (0x2DU)

static void lcd_delay(dfrobot_rgb_lcd_t *lcd, uint32_t delayUs)
{
    if ((lcd != NULL) && (lcd->config.delayUs != NULL))
    {
        lcd->config.delayUs(lcd->config.busContext, delayUs);
    }
}

static bool lcd_write(dfrobot_rgb_lcd_t *lcd, uint8_t address, const uint8_t *data, size_t length)
{
    bool ok;

    if ((lcd == NULL) || (lcd->config.write == NULL) || (data == NULL) || (length == 0U))
    {
        return false;
    }

    ok = lcd->config.write(lcd->config.busContext, address, data, length, lcd->config.timeout);
    lcd_delay(lcd, 100U);
    return ok;
}

static bool lcd_command(dfrobot_rgb_lcd_t *lcd, uint8_t command)
{
    uint8_t data[2] = {LCD_COMMAND_PREFIX, command};
    return lcd_write(lcd, lcd->config.lcdAddress, data, sizeof(data));
}

static bool rgb_set_reg(dfrobot_rgb_lcd_t *lcd, uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    return lcd_write(lcd, lcd->config.rgbAddress, data, sizeof(data));
}

static void rgb_select_registers(dfrobot_rgb_lcd_t *lcd)
{
    if (lcd->config.rgbAddress == RGB_ADDRESS_30_STYLE)
    {
        lcd->regRed = 0x06U;
        lcd->regGreen = 0x07U;
        lcd->regBlue = 0x08U;
    }
    else if (lcd->config.rgbAddress == RGB_ADDRESS_6B_STYLE)
    {
        lcd->regRed = 0x06U;
        lcd->regGreen = 0x05U;
        lcd->regBlue = 0x04U;
    }
    else if (lcd->config.rgbAddress == RGB_ADDRESS_2D_STYLE)
    {
        lcd->regRed = 0x01U;
        lcd->regGreen = 0x02U;
        lcd->regBlue = 0x03U;
    }
    else
    {
        lcd->regRed = 0x04U;
        lcd->regGreen = 0x03U;
        lcd->regBlue = 0x02U;
    }
}

static bool rgb_init(dfrobot_rgb_lcd_t *lcd)
{
    if (lcd->config.rgbAddress == RGB_ADDRESS_30_STYLE)
    {
        return rgb_set_reg(lcd, 0x01U, 0x00U) && rgb_set_reg(lcd, 0x02U, 0xFFU) && rgb_set_reg(lcd, 0x04U, 0x15U);
    }

    if (lcd->config.rgbAddress == RGB_ADDRESS_6B_STYLE)
    {
        return rgb_set_reg(lcd, 0x2FU, 0x00U) && rgb_set_reg(lcd, 0x00U, 0x20U) &&
               rgb_set_reg(lcd, 0x01U, 0x00U) && rgb_set_reg(lcd, 0x02U, 0x01U) &&
               rgb_set_reg(lcd, 0x03U, 0x04U);
    }

    return rgb_set_reg(lcd, 0x00U, 0x00U) && rgb_set_reg(lcd, 0x08U, 0xFFU) && rgb_set_reg(lcd, 0x01U, 0x20U);
}

void DFRobot_RgbLcd_GetDefaultConfig(dfrobot_rgb_lcd_config_t *config)
{
    if (config == NULL)
    {
        return;
    }

    (void)memset(config, 0, sizeof(*config));
    config->lcdAddress = DFROBOT_RGB_LCD_LCD_ADDRESS;
    config->rgbAddress = DFROBOT_RGB_LCD_RGB_ADDRESS;
    config->columns = DFROBOT_RGB_LCD_COLUMNS;
    config->rows = DFROBOT_RGB_LCD_ROWS;
    config->timeout = 1000000U;
}

bool DFRobot_RgbLcd_Init(dfrobot_rgb_lcd_t *lcd, const dfrobot_rgb_lcd_config_t *config)
{
    bool ok = true;

    if ((lcd == NULL) || (config == NULL) || (config->write == NULL) || (config->delayUs == NULL))
    {
        return false;
    }

    (void)memset(lcd, 0, sizeof(*lcd));
    lcd->config = *config;
    if (lcd->config.lcdAddress == 0U)
    {
        lcd->config.lcdAddress = DFROBOT_RGB_LCD_LCD_ADDRESS;
    }
    if (lcd->config.rgbAddress == 0U)
    {
        lcd->config.rgbAddress = DFROBOT_RGB_LCD_RGB_ADDRESS;
    }
    if (lcd->config.columns == 0U)
    {
        lcd->config.columns = DFROBOT_RGB_LCD_COLUMNS;
    }
    if (lcd->config.rows == 0U)
    {
        lcd->config.rows = DFROBOT_RGB_LCD_ROWS;
    }

    rgb_select_registers(lcd);
    lcd->showFunction = (uint8_t)(LCD_4BITMODE | LCD_2LINE | LCD_5X8DOTS);
    lcd->showControl = (uint8_t)(LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    lcd->showMode = (uint8_t)(LCD_ENTRYLEFT | LCD_ENTRYSHIFTDEC);

    lcd_delay(lcd, 50000U);
    ok = lcd_command(lcd, (uint8_t)(LCD_FUNCTIONSET | lcd->showFunction)) && ok;
    lcd_delay(lcd, 5000U);
    ok = lcd_command(lcd, (uint8_t)(LCD_FUNCTIONSET | lcd->showFunction)) && ok;
    lcd_delay(lcd, 5000U);
    ok = lcd_command(lcd, (uint8_t)(LCD_FUNCTIONSET | lcd->showFunction)) && ok;
    ok = lcd_command(lcd, (uint8_t)(LCD_DISPLAYCONTROL | lcd->showControl)) && ok;
    ok = DFRobot_RgbLcd_Clear(lcd) && ok;
    ok = lcd_command(lcd, (uint8_t)(LCD_ENTRYMODESET | lcd->showMode)) && ok;

    ok = rgb_init(lcd) && ok;
    ok = DFRobot_RgbLcd_SetRGB(lcd, 255U, 255U, 255U) && ok;

    return ok;
}

bool DFRobot_RgbLcd_Clear(dfrobot_rgb_lcd_t *lcd)
{
    bool ok = lcd_command(lcd, LCD_CLEARDISPLAY);
    lcd_delay(lcd, 2000U);
    return ok;
}

bool DFRobot_RgbLcd_SetCursor(dfrobot_rgb_lcd_t *lcd, uint8_t column, uint8_t row)
{
    static const uint8_t rowOffsets[] = {0x00U, 0x40U, 0x14U, 0x54U};

    if (lcd == NULL)
    {
        return false;
    }
    if (row >= lcd->config.rows)
    {
        row = (uint8_t)(lcd->config.rows - 1U);
    }
    if (column >= lcd->config.columns)
    {
        column = (uint8_t)(lcd->config.columns - 1U);
    }

    return lcd_command(lcd, (uint8_t)(LCD_SETDDRAMADDR | (column + rowOffsets[row])));
}

bool DFRobot_RgbLcd_WriteChar(dfrobot_rgb_lcd_t *lcd, char value)
{
    uint8_t data[2] = {LCD_DATA_PREFIX, (uint8_t)value};
    return lcd_write(lcd, lcd->config.lcdAddress, data, sizeof(data));
}

bool DFRobot_RgbLcd_WriteString(dfrobot_rgb_lcd_t *lcd, const char *text)
{
    if (text == NULL)
    {
        return false;
    }

    while (*text != '\0')
    {
        if (!DFRobot_RgbLcd_WriteChar(lcd, *text))
        {
            return false;
        }
        ++text;
    }

    return true;
}

bool DFRobot_RgbLcd_WriteLine(dfrobot_rgb_lcd_t *lcd, uint8_t row, const char *text)
{
    uint8_t column;

    if ((lcd == NULL) || (text == NULL) || !DFRobot_RgbLcd_SetCursor(lcd, 0U, row))
    {
        return false;
    }

    for (column = 0U; column < lcd->config.columns; ++column)
    {
        char value = (*text != '\0') ? *text++ : ' ';
        if (!DFRobot_RgbLcd_WriteChar(lcd, value))
        {
            return false;
        }
    }

    return true;
}

bool DFRobot_RgbLcd_SetRGB(dfrobot_rgb_lcd_t *lcd, uint8_t red, uint8_t green, uint8_t blue)
{
    if (lcd == NULL)
    {
        return false;
    }

    if (lcd->config.rgbAddress == RGB_ADDRESS_30_STYLE)
    {
        red = (uint8_t)(((uint16_t)red * 192U) / 255U);
        green = (uint8_t)(((uint16_t)green * 192U) / 255U);
        blue = (uint8_t)(((uint16_t)blue * 192U) / 255U);
    }

    if ((lcd->config.rgbAddress == RGB_ADDRESS_6B_STYLE) && !rgb_set_reg(lcd, 0x07U, 0xFFU))
    {
        return false;
    }

    return rgb_set_reg(lcd, lcd->regRed, red) && rgb_set_reg(lcd, lcd->regGreen, green) &&
           rgb_set_reg(lcd, lcd->regBlue, blue);
}
