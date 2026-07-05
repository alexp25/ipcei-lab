#ifndef DFROBOT_LCD_SHIELD_H_
#define DFROBOT_LCD_SHIELD_H_

#include "dfrobot_rgb_lcd.h"

#include <stdbool.h>
#include <stdint.h>

#define DFROBOT_LCD_SHIELD_I2C_BAUDRATE_HZ (100000U)
#define DFROBOT_LCD_SHIELD_RGB_ADDRESS     DFROBOT_RGB_LCD_RGB_ADDRESS

typedef struct
{
    dfrobot_rgb_lcd_t lcd;
} dfrobot_lcd_shield_t;

void DFRobot_LcdShieldBusInit(void);
bool DFRobot_LcdShieldProbeAddress(uint8_t address);
bool DFRobot_LcdShieldInit(dfrobot_lcd_shield_t *shield);

#endif /* DFROBOT_LCD_SHIELD_H_ */
