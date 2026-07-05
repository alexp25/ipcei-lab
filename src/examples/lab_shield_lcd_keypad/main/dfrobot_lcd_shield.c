#include "dfrobot_lcd_shield.h"

#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_lpi2c.h"

#include <string.h>

#define DFROBOT_LCD_SHIELD_LPI2C_BASE             LPI2C0
#define DFROBOT_LCD_SHIELD_TRANSFER_TIMEOUT_LOOPS (1000000U)

typedef struct
{
    LPI2C_Type *base;
    bool initialized;
} dfrobot_rgb_lcd_bus_t;

static dfrobot_rgb_lcd_bus_t s_bus;

static bool dfrobot_rgb_lcd_i2c_write(void *context, uint8_t address, const uint8_t *data, size_t length, uint32_t timeout)
{
    dfrobot_rgb_lcd_bus_t *bus = (dfrobot_rgb_lcd_bus_t *)context;
    lpi2c_master_transfer_t transfer;

    (void)timeout;

    if ((bus == NULL) || (data == NULL) || (length == 0U))
    {
        return false;
    }

    (void)memset(&transfer, 0, sizeof(transfer));
    transfer.slaveAddress = address;
    transfer.direction = kLPI2C_Write;
    transfer.subaddress = 0U;
    transfer.subaddressSize = 0U;
    transfer.data = (uint8_t *)data;
    transfer.dataSize = length;
    transfer.flags = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(bus->base, &transfer) == kStatus_Success;
}

static void dfrobot_rgb_lcd_delay_us(void *context, uint32_t delayUs)
{
    (void)context;
    SDK_DelayAtLeastUs(delayUs, SystemCoreClock);
}

void DFRobot_LcdShieldBusInit(void)
{
    lpi2c_master_config_t masterConfig;

    if (s_bus.initialized)
    {
        return;
    }

    CLOCK_SetClockDiv(kCLOCK_DivLPI2C0, 1U);
    CLOCK_AttachClk(kFRO12M_to_LPI2C0);

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = DFROBOT_LCD_SHIELD_I2C_BAUDRATE_HZ;

    s_bus.base = DFROBOT_LCD_SHIELD_LPI2C_BASE;
    LPI2C_MasterInit(s_bus.base, &masterConfig, CLOCK_GetLpi2cClkFreq());
    s_bus.initialized = true;
}

bool DFRobot_LcdShieldProbeAddress(uint8_t address)
{
    status_t status;

    DFRobot_LcdShieldBusInit();
    status = LPI2C_MasterStart(s_bus.base, address, kLPI2C_Write);
    (void)LPI2C_MasterStop(s_bus.base);

    return status == kStatus_Success;
}

bool DFRobot_LcdShieldInit(dfrobot_lcd_shield_t *shield)
{
    dfrobot_rgb_lcd_config_t lcdConfig;

    if (shield == NULL)
    {
        return false;
    }

    DFRobot_LcdShieldBusInit();

    DFRobot_RgbLcd_GetDefaultConfig(&lcdConfig);
    lcdConfig.busContext = &s_bus;
    lcdConfig.write = dfrobot_rgb_lcd_i2c_write;
    lcdConfig.delayUs = dfrobot_rgb_lcd_delay_us;
    lcdConfig.rgbAddress = DFROBOT_LCD_SHIELD_RGB_ADDRESS;
    lcdConfig.timeout = DFROBOT_LCD_SHIELD_TRANSFER_TIMEOUT_LOOPS;

    return DFRobot_RgbLcd_Init(&shield->lcd, &lcdConfig);
}
