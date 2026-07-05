#include "mfs_outputs.h"

#include <stddef.h>

static void write_pin(const mfs_gpio_pin_t *pin, bool asserted)
{
    GPIO_PinWrite(pin->gpio, pin->pin, MFS_PinLogic(pin, asserted));
}

void MFS_LedsInit(mfs_leds_t *leds, const mfs_gpio_pin_t pins[MFS_LED_COUNT])
{
    if ((leds == NULL) || (pins == NULL))
    {
        return;
    }

    for (uint8_t i = 0U; i < MFS_LED_COUNT; i++)
    {
        leds->pins[i] = pins[i];
        write_pin(&leds->pins[i], false);
    }
}

void MFS_LedSet(mfs_leds_t *leds, uint8_t index, bool on)
{
    if ((leds == NULL) || (index >= MFS_LED_COUNT))
    {
        return;
    }

    write_pin(&leds->pins[index], on);
}

void MFS_LedToggle(mfs_leds_t *leds, uint8_t index)
{
    if ((leds == NULL) || (index >= MFS_LED_COUNT))
    {
        return;
    }

    GPIO_PortToggle(leds->pins[index].gpio, 1UL << leds->pins[index].pin);
}

void MFS_BuzzerInit(mfs_buzzer_t *buzzer, const mfs_gpio_pin_t *pin)
{
    if ((buzzer == NULL) || (pin == NULL))
    {
        return;
    }

    buzzer->pin = *pin;
    write_pin(&buzzer->pin, false);
}

void MFS_BuzzerSet(mfs_buzzer_t *buzzer, bool on)
{
    if (buzzer == NULL)
    {
        return;
    }

    write_pin(&buzzer->pin, on);
}
