#ifndef LCD_KEYPAD_H_
#define LCD_KEYPAD_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    kLcdKeypadButtonRight,
    kLcdKeypadButtonUp,
    kLcdKeypadButtonDown,
    kLcdKeypadButtonLeft,
    kLcdKeypadButtonSelect,
    kLcdKeypadButtonNone
} lcd_keypad_button_t;

typedef bool (*lcd_keypad_read_adc_t)(void *context, uint16_t *value);

typedef struct
{
    uint16_t right_max;
    uint16_t up_max;
    uint16_t down_max;
    uint16_t left_max;
    uint16_t select_max;
} lcd_keypad_thresholds_t;

typedef struct
{
    void *context;
    lcd_keypad_read_adc_t read_adc;
    uint16_t adc_max_value;
    uint8_t stable_sample_count;
    lcd_keypad_thresholds_t thresholds_10bit;
} lcd_keypad_config_t;

typedef struct
{
    lcd_keypad_config_t config;
    lcd_keypad_button_t candidate;
    lcd_keypad_button_t stable;
    uint8_t candidate_count;
    uint16_t last_raw_value;
    uint16_t last_value_10bit;
} lcd_keypad_t;

extern const lcd_keypad_thresholds_t g_lcdKeypadDefaultThresholds;

bool LCD_KeypadInit(lcd_keypad_t *keypad, const lcd_keypad_config_t *config);
bool LCD_KeypadPoll(lcd_keypad_t *keypad, lcd_keypad_button_t *button);
uint16_t LCD_KeypadGetLastRawValue(const lcd_keypad_t *keypad);
uint16_t LCD_KeypadGetLast10BitValue(const lcd_keypad_t *keypad);
const char *LCD_KeypadButtonName(lcd_keypad_button_t button);

#endif /* LCD_KEYPAD_H_ */
