#ifndef MFS_COMMON_H_
#define MFS_COMMON_H_

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_reset.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct _mfs_gpio_pin
{
    GPIO_Type *gpio;
    PORT_Type *port;
    uint32_t pin;
    clock_ip_name_t portClock;
    clock_ip_name_t gpioClock;
    reset_ip_name_t portReset;
    reset_ip_name_t gpioReset;
    port_mux_t mux;
    bool activeLow;
} mfs_gpio_pin_t;

typedef struct _mfs_adc_channel
{
    uint32_t channel;
} mfs_adc_channel_t;

static inline bool MFS_PinIsAsserted(const mfs_gpio_pin_t *pin, uint32_t raw)
{
    return pin->activeLow ? (raw == 0U) : (raw != 0U);
}

static inline uint32_t MFS_PinLogic(const mfs_gpio_pin_t *pin, bool asserted)
{
    return (pin->activeLow == asserted) ? 0U : 1U;
}

#endif /* MFS_COMMON_H_ */
