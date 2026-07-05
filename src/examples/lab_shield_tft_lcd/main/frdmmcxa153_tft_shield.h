#ifndef FRDMMCXA153_TFT_SHIELD_H_
#define FRDMMCXA153_TFT_SHIELD_H_

#include "spfd5408_tft.h"

#include <stdbool.h>

typedef struct
{
    spfd5408_tft_config_t tft;
} frdmmcxa153_tft_shield_t;

bool FRDMMCXA153_TftShieldInit(frdmmcxa153_tft_shield_t *shield);

#endif /* FRDMMCXA153_TFT_SHIELD_H_ */
