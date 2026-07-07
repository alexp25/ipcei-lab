#ifndef DFROBOT_RGB_LCD_H_
#define DFROBOT_RGB_LCD_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DFROBOT_RGB_LCD_LCD_ADDRESS (0x3EU)
#define DFROBOT_RGB_LCD_RGB_ADDRESS (0x60U)
#define DFROBOT_RGB_LCD_COLUMNS     (16U)
#define DFROBOT_RGB_LCD_ROWS        (2U)

typedef bool (*dfrobot_rgb_lcd_i2c_write_t)(void *context,
                                                uint8_t address,
                                                const uint8_t *data,
                                                size_t length,
                                                uint32_t timeout);

typedef void (*dfrobot_rgb_lcd_delay_us_t)(void *context, uint32_t delayUs);

typedef struct
{
    void *busContext;
    dfrobot_rgb_lcd_i2c_write_t write;
    dfrobot_rgb_lcd_delay_us_t delayUs;
    uint8_t lcdAddress;
    uint8_t rgbAddress;
    uint8_t columns;
    uint8_t rows;
    uint32_t timeout;
} dfrobot_rgb_lcd_config_t;

typedef struct
{
    dfrobot_rgb_lcd_config_t config;
    uint8_t showFunction;
    uint8_t showControl;
    uint8_t showMode;
    uint8_t regRed;
    uint8_t regGreen;
    uint8_t regBlue;
} dfrobot_rgb_lcd_t;

void DFRobot_RgbLcd_GetDefaultConfig(dfrobot_rgb_lcd_config_t *config);
bool DFRobot_RgbLcd_Init(dfrobot_rgb_lcd_t *lcd, const dfrobot_rgb_lcd_config_t *config);
bool DFRobot_RgbLcd_Clear(dfrobot_rgb_lcd_t *lcd);
bool DFRobot_RgbLcd_SetCursor(dfrobot_rgb_lcd_t *lcd, uint8_t column, uint8_t row);
bool DFRobot_RgbLcd_WriteChar(dfrobot_rgb_lcd_t *lcd, char value);
bool DFRobot_RgbLcd_WriteString(dfrobot_rgb_lcd_t *lcd, const char *text);
bool DFRobot_RgbLcd_WriteLine(dfrobot_rgb_lcd_t *lcd, uint8_t row, const char *text);
bool DFRobot_RgbLcd_SetRGB(dfrobot_rgb_lcd_t *lcd, uint8_t red, uint8_t green, uint8_t blue);

#endif /* DFROBOT_RGB_LCD_H_ */
