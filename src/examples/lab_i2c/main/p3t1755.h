#ifndef P3T1755_H_
#define P3T1755_H_

#include "fsl_common.h"

#include <stddef.h>
#include <stdint.h>

#define P3T1755_I2C_ADDRESS       (0x48U)
#define P3T1755_REG_TEMPERATURE   (0x00U)
#define P3T1755_REG_CONFIGURATION (0x01U)
#define P3T1755_REG_TLOW          (0x02U)
#define P3T1755_REG_THIGH         (0x03U)

typedef status_t (*p3t1755_read_transfer_t)(void *context,
                                           uint8_t deviceAddress,
                                           uint8_t registerAddress,
                                           uint8_t *data,
                                           size_t dataSize,
                                           uint32_t timeout);

typedef status_t (*p3t1755_write_transfer_t)(void *context,
                                            uint8_t deviceAddress,
                                            uint8_t registerAddress,
                                            uint8_t *data,
                                            size_t dataSize,
                                            uint32_t timeout);

typedef struct
{
    void *busContext;
    p3t1755_read_transfer_t readTransfer;
    p3t1755_write_transfer_t writeTransfer;
    uint8_t deviceAddress;
    uint32_t timeout;
} p3t1755_config_t;

typedef struct
{
    p3t1755_config_t config;
    uint8_t address;
} p3t1755_t;

status_t p3t1755_init(p3t1755_t *sensor, const p3t1755_config_t *config);
status_t p3t1755_read_register(p3t1755_t *sensor, uint8_t registerAddress, uint8_t *data, size_t dataSize);
status_t p3t1755_write_register(p3t1755_t *sensor, uint8_t registerAddress, uint8_t *data, size_t dataSize);
status_t p3t1755_read_raw(p3t1755_t *sensor, int16_t *raw);
status_t p3t1755_read_celsius(p3t1755_t *sensor, float *temperatureCelsius);

int16_t p3t1755_decode_raw(uint8_t msb, uint8_t lsb);
float p3t1755_raw_to_celsius(uint8_t msb, uint8_t lsb);

#endif /* P3T1755_H_ */
