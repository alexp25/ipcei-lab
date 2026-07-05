#ifndef MFS_74HC595_DISPLAY_H_
#define MFS_74HC595_DISPLAY_H_

#include "mfs_common.h"

#include <stdbool.h>
#include <stdint.h>

#define MFS_DISPLAY_DIGITS 4U

typedef struct _mfs_display_map
{
    uint8_t digitSelect[MFS_DISPLAY_DIGITS];
    uint8_t segmentPattern[16];
    uint8_t blankPattern;
    uint8_t minusPattern;
    bool segmentsActiveLow;
    bool digitsActiveLow;
    bool shiftMsbFirst;
    bool digitByteFirst;
} mfs_display_map_t;

typedef struct _mfs_display
{
    mfs_gpio_pin_t latch;
    mfs_gpio_pin_t clock;
    mfs_gpio_pin_t data;
    mfs_display_map_t map;
    uint8_t digitPattern[MFS_DISPLAY_DIGITS];
    bool digitEnabled[MFS_DISPLAY_DIGITS];
    uint8_t refreshIndex;
} mfs_display_t;

void MFS_DisplayInit(mfs_display_t *display, const mfs_gpio_pin_t *latch, const mfs_gpio_pin_t *clock,
                     const mfs_gpio_pin_t *data, const mfs_display_map_t *map);
void MFS_DisplaySetNumber(mfs_display_t *display, int value);
void MFS_DisplaySetRaw(mfs_display_t *display, const uint8_t patterns[MFS_DISPLAY_DIGITS],
                       const bool enabled[MFS_DISPLAY_DIGITS]);
void MFS_DisplayRefresh(mfs_display_t *display);

extern const mfs_display_map_t g_mfsDisplayMapCommonCathode;

#endif /* MFS_74HC595_DISPLAY_H_ */
