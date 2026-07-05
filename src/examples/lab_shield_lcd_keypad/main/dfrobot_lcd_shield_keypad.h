#ifndef DFROBOT_LCD_SHIELD_KEYPAD_H_
#define DFROBOT_LCD_SHIELD_KEYPAD_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    kDFRobotLcdShieldButtonRight,
    kDFRobotLcdShieldButtonUp,
    kDFRobotLcdShieldButtonDown,
    kDFRobotLcdShieldButtonLeft,
    kDFRobotLcdShieldButtonSelect,
    kDFRobotLcdShieldButtonNone
} dfrobot_lcd_shield_button_t;

typedef struct
{
    uint16_t right_max;
    uint16_t up_max;
    uint16_t down_max;
    uint16_t left_max;
    uint16_t select_max;
} dfrobot_lcd_shield_keypad_thresholds_t;

typedef struct
{
    dfrobot_lcd_shield_keypad_thresholds_t thresholds;
    dfrobot_lcd_shield_button_t candidate;
    dfrobot_lcd_shield_button_t stable;
    uint8_t candidate_count;
    uint8_t stable_sample_count;
    uint16_t last_raw_value;
} dfrobot_lcd_shield_keypad_t;

bool DFRobot_LcdShieldKeypadInit(dfrobot_lcd_shield_keypad_t *keypad);
bool DFRobot_LcdShieldKeypadPoll(dfrobot_lcd_shield_keypad_t *keypad, dfrobot_lcd_shield_button_t *button);
uint16_t DFRobot_LcdShieldKeypadGetLastRawValue(const dfrobot_lcd_shield_keypad_t *keypad);
const char *DFRobot_LcdShieldKeypadButtonName(dfrobot_lcd_shield_button_t button);

#endif /* DFROBOT_LCD_SHIELD_KEYPAD_H_ */