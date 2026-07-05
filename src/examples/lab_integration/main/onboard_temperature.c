#include "onboard_temperature.h"

#include "fsl_clock.h"
#include "fsl_lpi2c.h"
#include "p3t1755.h"

#include <stdbool.h>
#include <string.h>

#define P3T1755_LPI2C_BASE              LPI2C0
#define P3T1755_I2C_BAUDRATE_HZ         (100000U)
#define P3T1755_TRANSFER_TIMEOUT_LOOPS   (1000000U)

typedef struct
{
    LPI2C_Type *base;
    lpi2c_master_handle_t masterHandle;
    volatile bool transferComplete;
    volatile status_t transferStatus;
} onboard_temperature_bus_t;

static onboard_temperature_bus_t s_bus;
static p3t1755_t s_sensor;

static void onboard_temperature_transfer_callback(LPI2C_Type *base,
                                                  lpi2c_master_handle_t *handle,
                                                  status_t completionStatus,
                                                  void *userData)
{
    onboard_temperature_bus_t *bus = (onboard_temperature_bus_t *)userData;

    (void)base;
    (void)handle;
    bus->transferStatus = completionStatus;
    bus->transferComplete = true;
}

static status_t onboard_temperature_transfer(void *context,
                                             uint8_t deviceAddress,
                                             uint8_t registerAddress,
                                             uint8_t *data,
                                             size_t dataSize,
                                             uint32_t timeout,
                                             lpi2c_direction_t direction)
{
    onboard_temperature_bus_t *bus = (onboard_temperature_bus_t *)context;
    lpi2c_master_transfer_t transfer;
    status_t status;

    if ((bus == NULL) || (data == NULL) || (dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    (void)memset(&transfer, 0, sizeof(transfer));
    transfer.slaveAddress = deviceAddress;
    transfer.direction = direction;
    transfer.subaddress = registerAddress;
    transfer.subaddressSize = 1U;
    transfer.data = data;
    transfer.dataSize = dataSize;
    transfer.flags = kLPI2C_TransferDefaultFlag;

    bus->transferComplete = false;
    bus->transferStatus = kStatus_Success;

    status = LPI2C_MasterTransferNonBlocking(bus->base, &bus->masterHandle, &transfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    while ((!bus->transferComplete) && (timeout > 0U))
    {
        --timeout;
    }

    if (!bus->transferComplete)
    {
        LPI2C_MasterTransferAbort(bus->base, &bus->masterHandle);
        return kStatus_Timeout;
    }

    return bus->transferStatus;
}

static status_t onboard_temperature_read(void *context,
                                         uint8_t deviceAddress,
                                         uint8_t registerAddress,
                                         uint8_t *data,
                                         size_t dataSize,
                                         uint32_t timeout)
{
    return onboard_temperature_transfer(context, deviceAddress, registerAddress, data, dataSize, timeout, kLPI2C_Read);
}

static status_t onboard_temperature_write(void *context,
                                          uint8_t deviceAddress,
                                          uint8_t registerAddress,
                                          uint8_t *data,
                                          size_t dataSize,
                                          uint32_t timeout)
{
    return onboard_temperature_transfer(context, deviceAddress, registerAddress, data, dataSize, timeout, kLPI2C_Write);
}

status_t onboard_temperature_init(void)
{
    lpi2c_master_config_t masterConfig;
    p3t1755_config_t sensorConfig;

    CLOCK_SetClockDiv(kCLOCK_DivLPI2C0, 1U);
    CLOCK_AttachClk(kFRO_HF_DIV_to_LPI2C0);

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = P3T1755_I2C_BAUDRATE_HZ;

    s_bus.base = P3T1755_LPI2C_BASE;
    LPI2C_MasterInit(s_bus.base, &masterConfig, CLOCK_GetLpi2cClkFreq());
    LPI2C_MasterTransferCreateHandle(s_bus.base, &s_bus.masterHandle, onboard_temperature_transfer_callback, &s_bus);

    sensorConfig.busContext = &s_bus;
    sensorConfig.readTransfer = onboard_temperature_read;
    sensorConfig.writeTransfer = onboard_temperature_write;
    sensorConfig.deviceAddress = P3T1755_I2C_ADDRESS;
    sensorConfig.timeout = P3T1755_TRANSFER_TIMEOUT_LOOPS;

    return p3t1755_init(&s_sensor, &sensorConfig);
}

status_t onboard_temperature_read_celsius(float *temperatureCelsius)
{
    return p3t1755_read_celsius(&s_sensor, temperatureCelsius);
}
