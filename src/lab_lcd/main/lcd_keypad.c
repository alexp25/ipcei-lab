#include "lcd_keypad.h"

#include <stddef.h>

#define LCD_KEYPAD_10BIT_MAX 1023U

const lcd_keypad_thresholds_t g_lcdKeypadDefaultThresholds = {
    .right_max = 80U,
    .up_max = 200U,
    .down_max = 400U,
    .left_max = 600U,
    .select_max = 800U,
};

static uint16_t LCD_KeypadScaleTo10Bit(const lcd_keypad_t *keypad, uint16_t raw_value)
{
    return (uint16_t)((((uint32_t)raw_value * LCD_KEYPAD_10BIT_MAX) +
                       (keypad->config.adc_max_value / 2U)) /
                      keypad->config.adc_max_value);
}

static lcd_keypad_button_t LCD_KeypadDecode(const lcd_keypad_t *keypad, uint16_t value_10bit)
{
    if (value_10bit <= keypad->config.thresholds_10bit.right_max)
    {
        return kLcdKeypadButtonRight;
    }
    if (value_10bit <= keypad->config.thresholds_10bit.up_max)
    {
        return kLcdKeypadButtonUp;
    }
    if (value_10bit <= keypad->config.thresholds_10bit.down_max)
    {
        return kLcdKeypadButtonDown;
    }
    if (value_10bit <= keypad->config.thresholds_10bit.left_max)
    {
        return kLcdKeypadButtonLeft;
    }
    if (value_10bit <= keypad->config.thresholds_10bit.select_max)
    {
        return kLcdKeypadButtonSelect;
    }
    return kLcdKeypadButtonNone;
}

bool LCD_KeypadInit(lcd_keypad_t *keypad, const lcd_keypad_config_t *config)
{
    if ((keypad == NULL) || (config == NULL) || (config->read_adc == NULL) ||
        (config->adc_max_value == 0U) || (config->stable_sample_count == 0U))
    {
        return false;
    }

    keypad->config = *config;
    keypad->candidate = kLcdKeypadButtonNone;
    keypad->stable = kLcdKeypadButtonNone;
    keypad->candidate_count = 0U;
    keypad->last_raw_value = 0U;
    keypad->last_value_10bit = 0U;
    return true;
}

bool LCD_KeypadPoll(lcd_keypad_t *keypad, lcd_keypad_button_t *button)
{
    uint16_t raw_value;
    lcd_keypad_button_t sample;

    if ((keypad == NULL) || (button == NULL) ||
        !keypad->config.read_adc(keypad->config.context, &raw_value))
    {
        return false;
    }

    keypad->last_raw_value = raw_value;
    keypad->last_value_10bit = LCD_KeypadScaleTo10Bit(keypad, raw_value);
    sample = LCD_KeypadDecode(keypad, keypad->last_value_10bit);
    if (sample != keypad->candidate)
    {
        keypad->candidate = sample;
        keypad->candidate_count = 1U;
    }
    else if (keypad->candidate_count < keypad->config.stable_sample_count)
    {
        keypad->candidate_count++;
    }

    if (keypad->candidate_count >= keypad->config.stable_sample_count)
    {
        keypad->stable = keypad->candidate;
    }

    *button = keypad->stable;
    return true;
}

uint16_t LCD_KeypadGetLastRawValue(const lcd_keypad_t *keypad)
{
    return (keypad != NULL) ? keypad->last_raw_value : 0U;
}

uint16_t LCD_KeypadGetLast10BitValue(const lcd_keypad_t *keypad)
{
    return (keypad != NULL) ? keypad->last_value_10bit : 0U;
}

const char *LCD_KeypadButtonName(lcd_keypad_button_t button)
{
    static const char *const names[] = {
        "RIGHT",
        "UP",
        "DOWN",
        "LEFT",
        "SELECT",
        "NONE",
    };

    if ((uint32_t)button >= (sizeof(names) / sizeof(names[0])))
    {
        return "NONE";
    }
    return names[button];
}
