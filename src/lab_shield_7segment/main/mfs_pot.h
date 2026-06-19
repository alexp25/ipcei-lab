#ifndef MFS_POT_H_
#define MFS_POT_H_

#include "mfs_common.h"

#include "fsl_common.h"

#include <stdint.h>

status_t MFS_PotInit(mfs_adc_channel_t channel);
status_t MFS_PotReadRaw(uint16_t *rawValue);

#endif /* MFS_POT_H_ */
