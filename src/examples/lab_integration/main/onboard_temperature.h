#ifndef ONBOARD_TEMPERATURE_H_
#define ONBOARD_TEMPERATURE_H_

#include "fsl_common.h"

status_t onboard_temperature_init(void);
status_t onboard_temperature_read_celsius(float *temperatureCelsius);

#endif /* ONBOARD_TEMPERATURE_H_ */
