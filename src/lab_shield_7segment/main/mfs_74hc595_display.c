#include "mfs_74hc595_display.h"

#include <stddef.h>

const mfs_display_map_t g_mfsDisplayMapCommonCathode = {
    .digitSelect = {0x01U, 0x02U, 0x04U, 0x08U},
    .segmentPattern = {0x3FU, 0x06U, 0x5BU, 0x4FU, 0x66U, 0x6DU, 0x7DU, 0x07U,
                       0x7FU, 0x6FU, 0x77U, 0x7CU, 0x39U, 0x5EU, 0x79U, 0x71U},
    .blankPattern = 0x00U,
    .minusPattern = 0x40U,
    .segmentsActiveLow = false,
    .digitsActiveLow = false,
    .shiftMsbFirst = true,
    .digitByteFirst = true,
};

static void write_pin(const mfs_gpio_pin_t *pin, bool asserted)
{
    GPIO_PinWrite(pin->gpio, pin->pin, MFS_PinLogic(pin, asserted));
}

static uint8_t apply_polarity(uint8_t value, bool activeLow)
{
    return activeLow ? (uint8_t)~value : value;
}

static void shift_byte(mfs_display_t *display, uint8_t value)
{
    for (uint8_t bit = 0U; bit < 8U; bit++)
    {
        uint8_t mask = display->map.shiftMsbFirst ? (uint8_t)(0x80U >> bit) : (uint8_t)(1U << bit);

        write_pin(&display->clock, false);
        write_pin(&display->data, (value & mask) != 0U);
        write_pin(&display->clock, true);
    }
}

static void shift_frame(mfs_display_t *display, uint8_t digitByte, uint8_t segmentByte)
{
    write_pin(&display->latch, false);
    if (display->map.digitByteFirst)
    {
        shift_byte(display, digitByte);
        shift_byte(display, segmentByte);
    }
    else
    {
        shift_byte(display, segmentByte);
        shift_byte(display, digitByte);
    }
    write_pin(&display->latch, true);
}

void MFS_DisplayInit(mfs_display_t *display, const mfs_gpio_pin_t *latch, const mfs_gpio_pin_t *clock,
                     const mfs_gpio_pin_t *data, const mfs_display_map_t *map)
{
    if ((display == NULL) || (latch == NULL) || (clock == NULL) || (data == NULL))
    {
        return;
    }

    display->latch = *latch;
    display->clock = *clock;
    display->data = *data;
    display->map = (map != NULL) ? *map : g_mfsDisplayMapCommonCathode;
    display->refreshIndex = 0U;

    for (uint8_t i = 0U; i < MFS_DISPLAY_DIGITS; i++)
    {
        display->digitPattern[i] = display->map.blankPattern;
        display->digitEnabled[i] = false;
    }

    write_pin(&display->latch, false);
    write_pin(&display->clock, false);
    write_pin(&display->data, false);
    shift_frame(display, apply_polarity(0U, display->map.digitsActiveLow),
                apply_polarity(display->map.blankPattern, display->map.segmentsActiveLow));
}

void MFS_DisplaySetNumber(mfs_display_t *display, int value)
{
    uint32_t magnitude;
    bool negative;
    uint8_t pos = MFS_DISPLAY_DIGITS;

    if (display == NULL)
    {
        return;
    }

    negative = (value < 0);
    magnitude = negative ? (uint32_t)(-value) : (uint32_t)value;

    for (uint8_t i = 0U; i < MFS_DISPLAY_DIGITS; i++)
    {
        display->digitPattern[i] = display->map.blankPattern;
        display->digitEnabled[i] = false;
    }

    do
    {
        pos--;
        display->digitPattern[pos] = display->map.segmentPattern[magnitude % 10U];
        display->digitEnabled[pos] = true;
        magnitude /= 10U;
    } while ((magnitude != 0U) && (pos > 0U));

    if (negative && (pos > 0U))
    {
        display->digitPattern[pos - 1U] = display->map.minusPattern;
        display->digitEnabled[pos - 1U] = true;
    }
}

void MFS_DisplaySetRaw(mfs_display_t *display, const uint8_t patterns[MFS_DISPLAY_DIGITS],
                       const bool enabled[MFS_DISPLAY_DIGITS])
{
    if ((display == NULL) || (patterns == NULL))
    {
        return;
    }

    for (uint8_t i = 0U; i < MFS_DISPLAY_DIGITS; i++)
    {
        display->digitPattern[i] = patterns[i];
        display->digitEnabled[i] = (enabled == NULL) ? true : enabled[i];
    }
}

void MFS_DisplayRefresh(mfs_display_t *display)
{
    uint8_t digitByte;
    uint8_t segmentByte;

    if (display == NULL)
    {
        return;
    }

    digitByte = display->digitEnabled[display->refreshIndex] ? display->map.digitSelect[display->refreshIndex] : 0U;
    segmentByte = display->digitPattern[display->refreshIndex];

    shift_frame(display, apply_polarity(digitByte, display->map.digitsActiveLow),
                apply_polarity(segmentByte, display->map.segmentsActiveLow));

    display->refreshIndex++;
    if (display->refreshIndex >= MFS_DISPLAY_DIGITS)
    {
        display->refreshIndex = 0U;
    }
}
