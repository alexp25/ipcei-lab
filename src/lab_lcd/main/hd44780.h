#ifndef HD44780_H_
#define HD44780_H_

#include <stdbool.h>
#include <stdint.h>

typedef void (*hd44780_control_write_t)(void *context, bool high);
typedef void (*hd44780_data_write_t)(void *context, uint8_t nibble);
typedef void (*hd44780_delay_t)(void *context, uint32_t delay_us);

typedef struct
{
    void *context;
    hd44780_control_write_t write_rs;
    hd44780_control_write_t write_enable;
    hd44780_data_write_t write_data;
    hd44780_delay_t delay_us;
} hd44780_config_t;

typedef struct
{
    hd44780_config_t config;
} hd44780_t;

bool HD44780_Init(hd44780_t *lcd, const hd44780_config_t *config);
void HD44780_Clear(hd44780_t *lcd);
void HD44780_SetCursor(hd44780_t *lcd, uint8_t column, uint8_t row);
void HD44780_WriteChar(hd44780_t *lcd, char character);
void HD44780_WriteString(hd44780_t *lcd, const char *text);
void HD44780_WriteLine(hd44780_t *lcd, uint8_t row, const char *text);

#endif /* HD44780_H_ */
