#ifndef MFS_OUTPUTS_H_
#define MFS_OUTPUTS_H_

#include "mfs_common.h"

#include <stdbool.h>
#include <stdint.h>

#define MFS_LED_COUNT 4U

typedef struct _mfs_leds
{
    mfs_gpio_pin_t pins[MFS_LED_COUNT];
} mfs_leds_t;

typedef struct _mfs_buzzer
{
    mfs_gpio_pin_t pin;
} mfs_buzzer_t;

void MFS_LedsInit(mfs_leds_t *leds, const mfs_gpio_pin_t pins[MFS_LED_COUNT]);
void MFS_LedSet(mfs_leds_t *leds, uint8_t index, bool on);
void MFS_LedToggle(mfs_leds_t *leds, uint8_t index);
void MFS_BuzzerInit(mfs_buzzer_t *buzzer, const mfs_gpio_pin_t *pin);
void MFS_BuzzerSet(mfs_buzzer_t *buzzer, bool on);

#endif /* MFS_OUTPUTS_H_ */
