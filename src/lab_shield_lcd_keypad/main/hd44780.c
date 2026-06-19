#include "hd44780.h"

#include <stddef.h>

#define HD44780_CLEAR_DISPLAY 0x01U
#define HD44780_ENTRY_MODE    0x06U
#define HD44780_DISPLAY_ON    0x0CU
#define HD44780_FUNCTION_SET  0x28U

#define HD44780_COLUMNS 16U
#define HD44780_ROWS    2U

static void HD44780_PulseEnable(hd44780_t *lcd)
{
    lcd->config.write_enable(lcd->config.context, false);
    lcd->config.delay_us(lcd->config.context, 2U);
    lcd->config.write_enable(lcd->config.context, true);
    lcd->config.delay_us(lcd->config.context, 2U);
    lcd->config.write_enable(lcd->config.context, false);
    lcd->config.delay_us(lcd->config.context, 100U);
}

static void HD44780_WriteNibble(hd44780_t *lcd, uint8_t nibble)
{
    lcd->config.write_data(lcd->config.context, nibble & 0x0FU);
    lcd->config.delay_us(lcd->config.context, 2U);
    HD44780_PulseEnable(lcd);
}

static void HD44780_WriteByte(hd44780_t *lcd, bool data, uint8_t value)
{
    lcd->config.write_rs(lcd->config.context, data);
    HD44780_WriteNibble(lcd, value >> 4U);
    HD44780_WriteNibble(lcd, value);
}

static void HD44780_WriteCommand(hd44780_t *lcd, uint8_t command)
{
    HD44780_WriteByte(lcd, false, command);
    if ((command == HD44780_CLEAR_DISPLAY) || (command == 0x02U))
    {
        lcd->config.delay_us(lcd->config.context, 2000U);
    }
}

bool HD44780_Init(hd44780_t *lcd, const hd44780_config_t *config)
{
    if ((lcd == NULL) || (config == NULL) || (config->write_rs == NULL) ||
        (config->write_enable == NULL) || (config->write_data == NULL) ||
        (config->delay_us == NULL))
    {
        return false;
    }

    lcd->config = *config;
    lcd->config.write_rs(lcd->config.context, false);
    lcd->config.write_enable(lcd->config.context, false);
    lcd->config.write_data(lcd->config.context, 0U);

    /* Some shield clones need substantially longer than the 40 ms minimum. */
    lcd->config.delay_us(lcd->config.context, 100000U);
    HD44780_WriteNibble(lcd, 0x03U);
    lcd->config.delay_us(lcd->config.context, 5000U);
    HD44780_WriteNibble(lcd, 0x03U);
    lcd->config.delay_us(lcd->config.context, 5000U);
    HD44780_WriteNibble(lcd, 0x03U);
    lcd->config.delay_us(lcd->config.context, 5000U);
    HD44780_WriteNibble(lcd, 0x02U);

    HD44780_WriteCommand(lcd, HD44780_FUNCTION_SET);
    HD44780_WriteCommand(lcd, HD44780_DISPLAY_ON);
    HD44780_Clear(lcd);
    HD44780_WriteCommand(lcd, HD44780_ENTRY_MODE);
    return true;
}

void HD44780_Clear(hd44780_t *lcd)
{
    HD44780_WriteCommand(lcd, HD44780_CLEAR_DISPLAY);
}

void HD44780_SetCursor(hd44780_t *lcd, uint8_t column, uint8_t row)
{
    static const uint8_t row_offsets[HD44780_ROWS] = {0x00U, 0x40U};

    if (row >= HD44780_ROWS)
    {
        row = HD44780_ROWS - 1U;
    }
    if (column >= HD44780_COLUMNS)
    {
        column = HD44780_COLUMNS - 1U;
    }
    HD44780_WriteCommand(lcd, 0x80U | (row_offsets[row] + column));
}

void HD44780_WriteChar(hd44780_t *lcd, char character)
{
    HD44780_WriteByte(lcd, true, (uint8_t)character);
}

void HD44780_WriteString(hd44780_t *lcd, const char *text)
{
    if (text == NULL)
    {
        return;
    }

    while (*text != '\0')
    {
        HD44780_WriteChar(lcd, *text);
        text++;
    }
}

void HD44780_WriteLine(hd44780_t *lcd, uint8_t row, const char *text)
{
    uint8_t column = 0U;

    HD44780_SetCursor(lcd, 0U, row);
    while ((text != NULL) && (*text != '\0') && (column < HD44780_COLUMNS))
    {
        HD44780_WriteChar(lcd, *text);
        text++;
        column++;
    }
    while (column < HD44780_COLUMNS)
    {
        HD44780_WriteChar(lcd, ' ');
        column++;
    }
}
