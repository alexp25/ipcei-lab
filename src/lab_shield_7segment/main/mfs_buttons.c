#include "mfs_buttons.h"

#include <stddef.h>

void MFS_ButtonsInit(mfs_buttons_t *buttons, const mfs_gpio_pin_t pins[MFS_BUTTON_COUNT], uint8_t thresholdTicks)
{
    if ((buttons == NULL) || (pins == NULL))
    {
        return;
    }

    buttons->thresholdTicks = (thresholdTicks == 0U) ? 1U : thresholdTicks;
    for (uint8_t i = 0U; i < MFS_BUTTON_COUNT; i++)
    {
        buttons->pins[i] = pins[i];
        buttons->integrator[i] = 0U;
        buttons->stable[i] = false;
        buttons->pressedEvent[i] = false;
    }
}

void MFS_ButtonsUpdate(mfs_buttons_t *buttons)
{
    if (buttons == NULL)
    {
        return;
    }

    for (uint8_t i = 0U; i < MFS_BUTTON_COUNT; i++)
    {
        bool rawPressed = MFS_PinIsAsserted(&buttons->pins[i], GPIO_PinRead(buttons->pins[i].gpio, buttons->pins[i].pin));

        if (rawPressed)
        {
            if (buttons->integrator[i] < buttons->thresholdTicks)
            {
                buttons->integrator[i]++;
            }
        }
        else if (buttons->integrator[i] > 0U)
        {
            buttons->integrator[i]--;
        }

        if (!buttons->stable[i] && (buttons->integrator[i] >= buttons->thresholdTicks))
        {
            buttons->stable[i] = true;
            buttons->pressedEvent[i] = true;
        }
        else if (buttons->stable[i] && (buttons->integrator[i] == 0U))
        {
            buttons->stable[i] = false;
        }
    }
}

bool MFS_ButtonsIsPressed(const mfs_buttons_t *buttons, uint8_t index)
{
    return ((buttons != NULL) && (index < MFS_BUTTON_COUNT)) ? buttons->stable[index] : false;
}

bool MFS_ButtonsTakePressed(mfs_buttons_t *buttons, uint8_t index)
{
    bool pressed;

    if ((buttons == NULL) || (index >= MFS_BUTTON_COUNT))
    {
        return false;
    }

    pressed = buttons->pressedEvent[index];
    buttons->pressedEvent[index] = false;
    return pressed;
}
