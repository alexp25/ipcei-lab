#ifndef MFS_BUTTONS_H_
#define MFS_BUTTONS_H_

#include "mfs_common.h"

#include <stdbool.h>
#include <stdint.h>

#define MFS_BUTTON_COUNT 3U

typedef struct _mfs_buttons
{
    mfs_gpio_pin_t pins[MFS_BUTTON_COUNT];
    uint8_t integrator[MFS_BUTTON_COUNT];
    bool stable[MFS_BUTTON_COUNT];
    bool pressedEvent[MFS_BUTTON_COUNT];
    uint8_t thresholdTicks;
} mfs_buttons_t;

void MFS_ButtonsInit(mfs_buttons_t *buttons, const mfs_gpio_pin_t pins[MFS_BUTTON_COUNT], uint8_t thresholdTicks);
void MFS_ButtonsUpdate(mfs_buttons_t *buttons);
bool MFS_ButtonsIsPressed(const mfs_buttons_t *buttons, uint8_t index);
bool MFS_ButtonsTakePressed(mfs_buttons_t *buttons, uint8_t index);

#endif /* MFS_BUTTONS_H_ */
