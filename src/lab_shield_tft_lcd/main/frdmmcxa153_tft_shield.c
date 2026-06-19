#include "frdmmcxa153_tft_shield.h"

#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_reset.h"

#include <stdint.h>

typedef struct
{
    PORT_Type *port;
    GPIO_Type *gpio;
    uint32_t pin;
} tft_pin_t;

/*
 * Classic Adafruit/MCUFRIEND shield bus used by SPFD5408_Adafruit_TFTLCD:
 * DB0=D8, DB1=D9, DB2=D2, DB3=D3, DB4=D4, DB5=D5, DB6=D6, DB7=D7.
 */
static const tft_pin_t s_dataPins[8] = {
    {PORT2, GPIO2, 7U},
    {PORT3, GPIO3, 31U},
    {PORT2, GPIO2, 4U},
    {PORT3, GPIO3, 0U},
    {PORT2, GPIO2, 5U},
    {PORT3, GPIO3, 12U},
    {PORT3, GPIO3, 13U},
    {PORT3, GPIO3, 1U},
};

/* Common MCUFRIEND/SPFD5408 shield control pins: A0-A4. */
static const tft_pin_t s_rdPin = {PORT1, GPIO1, 10U};
static const tft_pin_t s_wrPin = {PORT1, GPIO1, 12U};
static const tft_pin_t s_cdPin = {PORT1, GPIO1, 13U};
static const tft_pin_t s_csPin = {PORT2, GPIO2, 0U};

static void PinWrite(const tft_pin_t *pin, bool high)
{
    GPIO_PinWrite(pin->gpio, pin->pin, high ? 1U : 0U);
}

static void PulseWrite(void)
{
    PinWrite(&s_wrPin, false);
    SDK_DelayAtLeastUs(1U, SystemCoreClock);
    PinWrite(&s_wrPin, true);
}

static void WriteBus(uint8_t data)
{
    uint8_t bit;

    for (bit = 0U; bit < 8U; bit++)
    {
        GPIO_PinWrite(s_dataPins[bit].gpio, s_dataPins[bit].pin, (data >> bit) & 1U);
    }
    PulseWrite();
}

static void WriteCommand(void *context, uint8_t command)
{
    (void)context;
    PinWrite(&s_cdPin, false);
    WriteBus(command);
}

static void WriteData(void *context, uint8_t data)
{
    (void)context;
    PinWrite(&s_cdPin, true);
    WriteBus(data);
}

static void Reset(void *context)
{
    (void)context;
    PinWrite(&s_csPin, false);
    PinWrite(&s_wrPin, true);
    PinWrite(&s_rdPin, true);
    /*
     * Do not drive Arduino A4 as a dedicated TFT reset on FRDM-MCXA153: A4
     * shares P3_31 with Arduino D9, which this shield uses as LCD DB1.
     */
    SDK_DelayAtLeastUs(150000U, SystemCoreClock);
}

static void DelayMs(void *context, uint32_t delay_ms)
{
    (void)context;
    while (delay_ms > 0U)
    {
        SDK_DelayAtLeastUs(1000U, SystemCoreClock);
        delay_ms--;
    }
}

static void InitPin(const tft_pin_t *pin, bool initial_high)
{
    gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = initial_high ? 1U : 0U,
    };

    PORT_SetPinMux(pin->port, pin->pin, kPORT_MuxAlt0);
    GPIO_PinInit(pin->gpio, pin->pin, &config);
}

static void InitPins(void)
{
    uint8_t bit;

    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    CLOCK_EnableClock(kCLOCK_GatePORT3);
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    CLOCK_EnableClock(kCLOCK_GateGPIO2);
    CLOCK_EnableClock(kCLOCK_GateGPIO3);

    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);

    for (bit = 0U; bit < 8U; bit++)
    {
        InitPin(&s_dataPins[bit], bit == 1U);
    }

    InitPin(&s_rdPin, true);
    InitPin(&s_wrPin, true);
    InitPin(&s_cdPin, true);
    InitPin(&s_csPin, true);
}

bool FRDMMCXA153_TftShieldInit(frdmmcxa153_tft_shield_t *shield)
{
    if (shield == NULL)
    {
        return false;
    }

    InitPins();

    shield->tft.context = NULL;
    shield->tft.write_command = WriteCommand;
    shield->tft.write_data = WriteData;
    shield->tft.reset = Reset;
    shield->tft.delay_ms = DelayMs;
    return true;
}




