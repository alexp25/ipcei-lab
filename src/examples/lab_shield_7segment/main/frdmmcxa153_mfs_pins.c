#include "frdmmcxa153_mfs_pins.h"

#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_reset.h"

#include <stddef.h>

#define PIN(gpioBase, portBase, pinNumber, portClk, gpioClk, portRst, gpioRst, isActiveLow) \
    {                                                                                       \
        gpioBase, portBase, pinNumber, portClk, gpioClk, portRst, gpioRst, kPORT_MuxAlt0,   \
            isActiveLow                                                                     \
    }

#ifndef MFS_BUZZER_PIN_DRIVE_ENABLED
#define MFS_BUZZER_PIN_DRIVE_ENABLED 0
#endif

/*
 * FRDM-MCXA153 Arduino-compatible header mapping from the SDK board pin_mux:
 * BEEPER_PIN    D3  P1_6
 * LATCH_PIN     D4  P2_4
 * CLK_PIN       D7  P3_12
 * DATA_PIN      D8  P3_13
 * LED_1_PIN     D13 P2_6
 * LED_2_PIN     D12 P1_5
 * LED_3_PIN     D11 P1_4
 * LED_4_PIN     D10 P3_15
 * POT_PIN       A0  P1_10 ADC0_A8
 * BUTTON_1_PIN  A1  J4[4]  P1_12
 * BUTTON_2_PIN  A2  J4[6]  P1_13
 * BUTTON_3_PIN  A3  J4[8]  P2_0
 *
 * The generated board pin mux and the working hardware test both identify A0 as P1_10/ADC0_A8.
 * Some pinout diagrams show a different Arduino header variant; keep this table as the single
 * place to adjust those routes.
 *
 * The shield is an Arduino-style 5 V board. Drive the FRDM-MCXA153 Arduino
 * I/O at the board's 3.3 V logic rail, or add level shifting before applying
 * 5 V signals to MCU inputs. The potentiometer wiper on A0 must stay within
 * the ADC reference range.
 */
static const mfs_gpio_pin_t s_latch = PIN(GPIO2, PORT2, 4U, kCLOCK_GatePORT2, kCLOCK_GateGPIO2,
                                          kPORT2_RST_SHIFT_RSTn, kGPIO2_RST_SHIFT_RSTn, false);
static const mfs_gpio_pin_t s_clock = PIN(GPIO3, PORT3, 12U, kCLOCK_GatePORT3, kCLOCK_GateGPIO3,
                                          kPORT3_RST_SHIFT_RSTn, kGPIO3_RST_SHIFT_RSTn, false);
static const mfs_gpio_pin_t s_data = PIN(GPIO3, PORT3, 13U, kCLOCK_GatePORT3, kCLOCK_GateGPIO3,
                                         kPORT3_RST_SHIFT_RSTn, kGPIO3_RST_SHIFT_RSTn, false);
static const mfs_gpio_pin_t s_buzzer = PIN(GPIO1, PORT1, 6U, kCLOCK_GatePORT1, kCLOCK_GateGPIO1,
                                           kPORT1_RST_SHIFT_RSTn, kGPIO1_RST_SHIFT_RSTn, false);
static const mfs_gpio_pin_t s_ledPins[MFS_LED_COUNT] = {
    PIN(GPIO2, PORT2, 6U, kCLOCK_GatePORT2, kCLOCK_GateGPIO2, kPORT2_RST_SHIFT_RSTn, kGPIO2_RST_SHIFT_RSTn, false),
    PIN(GPIO1, PORT1, 5U, kCLOCK_GatePORT1, kCLOCK_GateGPIO1, kPORT1_RST_SHIFT_RSTn, kGPIO1_RST_SHIFT_RSTn, false),
    PIN(GPIO1, PORT1, 4U, kCLOCK_GatePORT1, kCLOCK_GateGPIO1, kPORT1_RST_SHIFT_RSTn, kGPIO1_RST_SHIFT_RSTn, false),
    PIN(GPIO3, PORT3, 15U, kCLOCK_GatePORT3, kCLOCK_GateGPIO3, kPORT3_RST_SHIFT_RSTn, kGPIO3_RST_SHIFT_RSTn, false),
};
static const mfs_gpio_pin_t s_buttonPins[MFS_BUTTON_COUNT] = {
    PIN(GPIO1, PORT1, 12U, kCLOCK_GatePORT1, kCLOCK_GateGPIO1, kPORT1_RST_SHIFT_RSTn, kGPIO1_RST_SHIFT_RSTn, true),
    PIN(GPIO1, PORT1, 13U, kCLOCK_GatePORT1, kCLOCK_GateGPIO1, kPORT1_RST_SHIFT_RSTn, kGPIO1_RST_SHIFT_RSTn, true),
    PIN(GPIO2, PORT2, 0U, kCLOCK_GatePORT2, kCLOCK_GateGPIO2, kPORT2_RST_SHIFT_RSTn, kGPIO2_RST_SHIFT_RSTn, true),
};
static const mfs_adc_channel_t s_potChannel = {.channel = 8U};

static void init_output_pin(const mfs_gpio_pin_t *pin, bool initialAsserted)
{
    gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = MFS_PinLogic(pin, initialAsserted),
    };

    CLOCK_EnableClock(pin->portClock);
    CLOCK_EnableClock(pin->gpioClock);
    RESET_ReleasePeripheralReset(pin->portReset);
    RESET_ReleasePeripheralReset(pin->gpioReset);
    PORT_SetPinMux(pin->port, pin->pin, pin->mux);
    pin->port->PCR[pin->pin] = ((pin->port->PCR[pin->pin] & ~(PORT_PCR_IBE_MASK)) | PORT_PCR_IBE(1U));
    GPIO_PinInit(pin->gpio, pin->pin, &config);
}

static void init_input_pullup_pin(const mfs_gpio_pin_t *pin)
{
    gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };

    CLOCK_EnableClock(pin->portClock);
    CLOCK_EnableClock(pin->gpioClock);
    RESET_ReleasePeripheralReset(pin->portReset);
    RESET_ReleasePeripheralReset(pin->gpioReset);
    PORT_SetPinMux(pin->port, pin->pin, pin->mux);
    pin->port->PCR[pin->pin] = ((pin->port->PCR[pin->pin] &
                                 ~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IBE_MASK)) |
                                PORT_PCR_PS(1U) | PORT_PCR_PE(1U) | PORT_PCR_IBE(1U));
    GPIO_PinInit(pin->gpio, pin->pin, &config);
}

static void init_input_pulldown_pin(const mfs_gpio_pin_t *pin)
{
    gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };

    CLOCK_EnableClock(pin->portClock);
    CLOCK_EnableClock(pin->gpioClock);
    RESET_ReleasePeripheralReset(pin->portReset);
    RESET_ReleasePeripheralReset(pin->gpioReset);
    PORT_SetPinMux(pin->port, pin->pin, pin->mux);
    pin->port->PCR[pin->pin] = ((pin->port->PCR[pin->pin] &
                                 ~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IBE_MASK)) |
                                PORT_PCR_PS(0U) | PORT_PCR_PE(1U) | PORT_PCR_IBE(1U));
    GPIO_PinInit(pin->gpio, pin->pin, &config);
}

status_t FRDMMCXA153_MFS_Init(frdmmcxa153_mfs_t *shield)
{
    if (shield == NULL)
    {
        return kStatus_InvalidArgument;
    }

    init_output_pin(&s_latch, false);
    init_output_pin(&s_clock, false);
    init_output_pin(&s_data, false);
#if MFS_BUZZER_PIN_DRIVE_ENABLED
    init_output_pin(&s_buzzer, false);
#else
    init_input_pulldown_pin(&s_buzzer);
#endif
    for (uint8_t i = 0U; i < MFS_LED_COUNT; i++)
    {
        init_output_pin(&s_ledPins[i], false);
    }
    for (uint8_t i = 0U; i < MFS_BUTTON_COUNT; i++)
    {
        init_input_pullup_pin(&s_buttonPins[i]);
    }

    MFS_DisplayInit(&shield->display, &s_latch, &s_clock, &s_data, &g_mfsDisplayMapCommonCathode);
    MFS_ButtonsInit(&shield->buttons, s_buttonPins, 5U);
    MFS_LedsInit(&shield->leds, s_ledPins);
    MFS_BuzzerInit(&shield->buzzer, &s_buzzer);

    return MFS_PotInit(s_potChannel);
}

void FRDMMCXA153_MFS_RefreshDisplay(frdmmcxa153_mfs_t *shield)
{
    if (shield != NULL)
    {
        MFS_DisplayRefresh(&shield->display);
    }
}

void FRDMMCXA153_MFS_UpdateButtons(frdmmcxa153_mfs_t *shield)
{
    if (shield != NULL)
    {
        MFS_ButtonsUpdate(&shield->buttons);
    }
}

status_t FRDMMCXA153_MFS_ReadPotRaw(uint16_t *rawValue)
{
    return MFS_PotReadRaw(rawValue);
}
