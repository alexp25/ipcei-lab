#ifndef FRDMMCXA153_LCD_SHIELD_H_
#define FRDMMCXA153_LCD_SHIELD_H_

#include "hd44780.h"
#include "lcd_keypad.h"

#include <stdbool.h>

#ifndef LCD_SHIELD_ENABLE_BACKLIGHT_CONTROL
#define LCD_SHIELD_ENABLE_BACKLIGHT_CONTROL 1
#endif

#ifndef LCD_SHIELD_BACKLIGHT_ACTIVE_HIGH
#define LCD_SHIELD_BACKLIGHT_ACTIVE_HIGH 1
#endif

typedef struct
{
    hd44780_config_t lcd;
    lcd_keypad_config_t keypad;
} frdmmcxa153_lcd_shield_t;

bool FRDMMCXA153_LcdShieldInit(frdmmcxa153_lcd_shield_t *shield);
void FRDMMCXA153_LcdShieldSetBacklight(bool on);

#endif /* FRDMMCXA153_LCD_SHIELD_H_ */
