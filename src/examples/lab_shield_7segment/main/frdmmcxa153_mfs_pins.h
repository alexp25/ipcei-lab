#ifndef FRDMMCXA153_MFS_PINS_H_
#define FRDMMCXA153_MFS_PINS_H_

#include "mfs_74hc595_display.h"
#include "mfs_buttons.h"
#include "mfs_outputs.h"
#include "mfs_pot.h"

#include "fsl_common.h"

typedef struct _frdmmcxa153_mfs
{
    mfs_display_t display;
    mfs_buttons_t buttons;
    mfs_leds_t leds;
    mfs_buzzer_t buzzer;
} frdmmcxa153_mfs_t;

status_t FRDMMCXA153_MFS_Init(frdmmcxa153_mfs_t *shield);
void FRDMMCXA153_MFS_RefreshDisplay(frdmmcxa153_mfs_t *shield);
void FRDMMCXA153_MFS_UpdateButtons(frdmmcxa153_mfs_t *shield);
status_t FRDMMCXA153_MFS_ReadPotRaw(uint16_t *rawValue);

#endif /* FRDMMCXA153_MFS_PINS_H_ */
