#include "board_ili9341.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_reset.h"
#include <string.h>

#define BOARD_ILI9341_SPI LPSPI0
#define BOARD_ILI9341_SPI_BAUDRATE_HZ 12000000U
#define BOARD_ILI9341_SPI_SOURCE_CLOCK_HZ 12000000U

/*
 * FRDM-MCXA153 <-> 2.4" ILI9341 SPI TFT wiring.
 *
 * The board pinout image labels the 12-pin Pmod-style SPI header as:
 *   J2[1]  P1_3  CS   -> LCD TFT_CS
 *   J2[3]  P1_0  SDO  -> LCD TFT_MOSI / SDI
 *   J2[5]  P1_2  SDI  -> LCD TFT_MISO / SDO, optional
 *   J2[7]  P1_1  SCK  -> LCD TFT_CLK / SCK
 *   J2[9]  GND        -> LCD GND
 *   J2[11] VDD        -> LCD VCC, use 3.3 V only
 *
 * Extra LCD control GPIOs are routed to available FRDM header pins:
 *   J1[2]  P1_4       -> LCD TFT_DC
 *   J1[4]  P1_5       -> LCD TFT_RST
 *   J1[3]  P1_6       -> LCD LED/backlight, optional
 *
 * The display module's Arduino-style names are used only as signal names.
 * This project does not use Arduino libraries, and the FRDM-MCXA153 pins
 * must not be driven with 5 V logic.
 */

/* Pmod-style SPI header: J2[1] / P1_3 / CS -> LCD TFT_CS. */
#define BOARD_ILI9341_CS_GPIO GPIO1
#define BOARD_ILI9341_CS_PIN  3U
/* FRDM header: J1[2] / P1_4 -> LCD TFT_DC. */
#define BOARD_ILI9341_DC_GPIO GPIO1
#define BOARD_ILI9341_DC_PIN  4U
/* FRDM header: J1[4] / P1_5 -> LCD TFT_RST. */
#define BOARD_ILI9341_RST_GPIO GPIO1
#define BOARD_ILI9341_RST_PIN  5U
/* FRDM header: J1[3] / P1_6 -> optional LCD LED/backlight control. */
#define BOARD_ILI9341_BL_GPIO GPIO1
#define BOARD_ILI9341_BL_PIN  6U

static void init_gpio_output(GPIO_Type *gpio, uint32_t pin, uint8_t initial_value)
{
    gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = initial_value
    };
    GPIO_PinInit(gpio, pin, &config);
}

void BOARD_InitIli9341Pins(void)
{
    /* Power the ILI9341 module from 3.3 V for direct FRDM-MCXA153 connection; do not drive these pins at 5 V. */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    CLOCK_EnableClock(kCLOCK_GateLPSPI0);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kLPSPI0_RST_SHIFT_RSTn);

    CLOCK_AttachClk(kFRO12M_to_LPSPI0);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI0, 1U);

    const port_pin_config_t spi_pin_config = {
        .pullSelect = kPORT_PullDisable,
        .pullValueSelect = kPORT_LowPullResistor,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_LowDriveStrength,
        .driveStrength1 = kPORT_NormalDriveStrength,
        .mux = kPORT_MuxAlt2,
        .inputBuffer = kPORT_InputBufferEnable,
        .invertInput = kPORT_InputNormal,
        .lockRegister = kPORT_UnlockRegister
    };

    PORT_SetPinConfig(PORT1, 0U, &spi_pin_config); /* J2[3] P1_0 SDO -> LPSPI0_SDO -> LCD TFT_MOSI. */
    PORT_SetPinConfig(PORT1, 1U, &spi_pin_config); /* J2[7] P1_1 SCK -> LPSPI0_SCK -> LCD TFT_CLK. */
    PORT_SetPinConfig(PORT1, 2U, &spi_pin_config); /* J2[5] P1_2 SDI -> LPSPI0_SDI -> LCD TFT_MISO, optional. */

    const port_pin_config_t gpio_pin_config = {
        .pullSelect = kPORT_PullDisable,
        .pullValueSelect = kPORT_LowPullResistor,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_LowDriveStrength,
        .driveStrength1 = kPORT_NormalDriveStrength,
        .mux = kPORT_MuxAlt0,
        .inputBuffer = kPORT_InputBufferEnable,
        .invertInput = kPORT_InputNormal,
        .lockRegister = kPORT_UnlockRegister
    };

    PORT_SetPinConfig(PORT1, BOARD_ILI9341_CS_PIN, &gpio_pin_config);  /* J2[1] P1_3 CS  -> LCD TFT_CS. */
    PORT_SetPinConfig(PORT1, BOARD_ILI9341_DC_PIN, &gpio_pin_config);  /* J1[2] P1_4     -> LCD TFT_DC. */
    PORT_SetPinConfig(PORT1, BOARD_ILI9341_RST_PIN, &gpio_pin_config); /* J1[4] P1_5     -> LCD TFT_RST. */
    PORT_SetPinConfig(PORT1, BOARD_ILI9341_BL_PIN, &gpio_pin_config);  /* J1[3] P1_6     -> optional LCD LED. */

    init_gpio_output(BOARD_ILI9341_CS_GPIO, BOARD_ILI9341_CS_PIN, 1U);
    init_gpio_output(BOARD_ILI9341_DC_GPIO, BOARD_ILI9341_DC_PIN, 1U);
    init_gpio_output(BOARD_ILI9341_RST_GPIO, BOARD_ILI9341_RST_PIN, 1U);
    init_gpio_output(BOARD_ILI9341_BL_GPIO, BOARD_ILI9341_BL_PIN, 1U);
}

void BOARD_GetIli9341Config(ili9341_config_t *config)
{
    memset(config, 0, sizeof(*config));
    config->spi = BOARD_ILI9341_SPI;
    config->spi_source_clock_hz = BOARD_ILI9341_SPI_SOURCE_CLOCK_HZ;
    config->spi_baudrate_hz = BOARD_ILI9341_SPI_BAUDRATE_HZ;
    config->transfer_timeout_loops = 1000000U;
    config->cs = (ili9341_gpio_pin_t){BOARD_ILI9341_CS_GPIO, BOARD_ILI9341_CS_PIN, true, true};
    config->dc = (ili9341_gpio_pin_t){BOARD_ILI9341_DC_GPIO, BOARD_ILI9341_DC_PIN, false, true};
    config->rst = (ili9341_gpio_pin_t){BOARD_ILI9341_RST_GPIO, BOARD_ILI9341_RST_PIN, true, true};
    config->backlight = (ili9341_gpio_pin_t){BOARD_ILI9341_BL_GPIO, BOARD_ILI9341_BL_PIN, false, true};
}
