#include "p3t1755.h"

#include <string.h>

status_t p3t1755_init(p3t1755_t *sensor, const p3t1755_config_t *config)
{
    uint8_t configuration;

    if ((sensor == NULL) || (config == NULL) || (config->readTransfer == NULL) ||
        (config->writeTransfer == NULL) || (config->timeout == 0U))
    {
        return kStatus_InvalidArgument;
    }

    (void)memset(sensor, 0, sizeof(*sensor));
    sensor->config = *config;
    sensor->address = config->deviceAddress;

    /* Probe the device without changing its power-on configuration. */
    return p3t1755_read_register(sensor, P3T1755_REG_CONFIGURATION, &configuration, sizeof(configuration));
}

status_t p3t1755_read_register(p3t1755_t *sensor, uint8_t registerAddress, uint8_t *data, size_t dataSize)
{
    if ((sensor == NULL) || (sensor->config.readTransfer == NULL) || (data == NULL) || (dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    return sensor->config.readTransfer(sensor->config.busContext, sensor->address, registerAddress, data, dataSize,
                                       sensor->config.timeout);
}

status_t p3t1755_write_register(p3t1755_t *sensor, uint8_t registerAddress, uint8_t *data, size_t dataSize)
{
    if ((sensor == NULL) || (sensor->config.writeTransfer == NULL) || (data == NULL) || (dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    return sensor->config.writeTransfer(sensor->config.busContext, sensor->address, registerAddress, data, dataSize,
                                        sensor->config.timeout);
}

int16_t p3t1755_decode_raw(uint8_t msb, uint8_t lsb)
{
    int16_t raw = (int16_t)((((uint16_t)msb << 8U) | (uint16_t)lsb) >> 4U);

    if ((raw & 0x0800) != 0)
    {
        raw |= (int16_t)0xF000;
    }

    return raw;
}

float p3t1755_raw_to_celsius(uint8_t msb, uint8_t lsb)
{
    return (float)p3t1755_decode_raw(msb, lsb) * 0.0625F;
}

status_t p3t1755_read_raw(p3t1755_t *sensor, int16_t *raw)
{
    status_t status;
    uint8_t data[2];

    if (raw == NULL)
    {
        return kStatus_InvalidArgument;
    }

    status = p3t1755_read_register(sensor, P3T1755_REG_TEMPERATURE, data, sizeof(data));
    if (status == kStatus_Success)
    {
        *raw = p3t1755_decode_raw(data[0], data[1]);
    }

    return status;
}

status_t p3t1755_read_celsius(p3t1755_t *sensor, float *temperatureCelsius)
{
    status_t status;
    int16_t raw;

    if (temperatureCelsius == NULL)
    {
        return kStatus_InvalidArgument;
    }

    status = p3t1755_read_raw(sensor, &raw);
    if (status == kStatus_Success)
    {
        *temperatureCelsius = (float)raw * 0.0625F;
    }

    return status;
}
