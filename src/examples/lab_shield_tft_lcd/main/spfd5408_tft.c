#include "spfd5408_tft.h"

#include <stddef.h>

#define TFT_CMD_SWRESET  0x01U
#define TFT_CMD_SLPOUT   0x11U
#define TFT_CMD_DISPON   0x29U
#define TFT_CMD_CASET    0x2AU
#define TFT_CMD_PASET    0x2BU
#define TFT_CMD_RAMWR    0x2CU
#define TFT_CMD_MADCTL   0x36U
#define TFT_CMD_PIXFMT   0x3AU
#define TFT_CMD_GAMMASET 0x26U

#define TFT_MADCTL_MY  0x80U
#define TFT_MADCTL_MX  0x40U
#define TFT_MADCTL_MV  0x20U
#define TFT_MADCTL_BGR 0x08U

static void WriteCommand(spfd5408_tft_t *tft, uint8_t command)
{
    tft->config.write_command(tft->config.context, command);
}

static void WriteData(spfd5408_tft_t *tft, uint8_t data)
{
    tft->config.write_data(tft->config.context, data);
}

static void WriteData16(spfd5408_tft_t *tft, uint16_t data)
{
    WriteData(tft, (uint8_t)(data >> 8U));
    WriteData(tft, (uint8_t)data);
}

static void WriteCommandData(spfd5408_tft_t *tft, uint8_t command, uint8_t data)
{
    WriteCommand(tft, command);
    WriteData(tft, data);
}

static void WriteCommandDataList(spfd5408_tft_t *tft, uint8_t command, const uint8_t *data, uint8_t length)
{
    uint8_t index;

    WriteCommand(tft, command);
    for (index = 0U; index < length; index++)
    {
        WriteData(tft, data[index]);
    }
}

static void RunIli9341Init(spfd5408_tft_t *tft)
{
    static const uint8_t power_control_b[] = {0x00U, 0xC1U, 0x30U};
    static const uint8_t power_on_sequence[] = {0x64U, 0x03U, 0x12U, 0x81U};
    static const uint8_t driver_timing_a[] = {0x85U, 0x00U, 0x78U};
    static const uint8_t power_control_a[] = {0x39U, 0x2CU, 0x00U, 0x34U, 0x02U};
    static const uint8_t pump_ratio[] = {0x20U};
    static const uint8_t driver_timing_b[] = {0x00U, 0x00U};
    static const uint8_t vcom_control[] = {0x3EU, 0x28U};
    static const uint8_t frame_control[] = {0x00U, 0x18U};
    static const uint8_t display_function[] = {0x08U, 0x82U, 0x27U};
    static const uint8_t positive_gamma[] = {
        0x0FU, 0x31U, 0x2BU, 0x0CU, 0x0EU, 0x08U, 0x4EU, 0xF1U,
        0x37U, 0x07U, 0x10U, 0x03U, 0x0EU, 0x09U, 0x00U,
    };
    static const uint8_t negative_gamma[] = {
        0x00U, 0x0EU, 0x14U, 0x03U, 0x11U, 0x07U, 0x31U, 0xC1U,
        0x48U, 0x08U, 0x0FU, 0x0CU, 0x31U, 0x36U, 0x0FU,
    };

    WriteCommandDataList(tft, 0xCFU, power_control_b, sizeof(power_control_b));
    WriteCommandDataList(tft, 0xEDU, power_on_sequence, sizeof(power_on_sequence));
    WriteCommandDataList(tft, 0xE8U, driver_timing_a, sizeof(driver_timing_a));
    WriteCommandDataList(tft, 0xCBU, power_control_a, sizeof(power_control_a));
    WriteCommandDataList(tft, 0xF7U, pump_ratio, sizeof(pump_ratio));
    WriteCommandDataList(tft, 0xEAU, driver_timing_b, sizeof(driver_timing_b));

    WriteCommandData(tft, 0xC0U, 0x23U);
    WriteCommandData(tft, 0xC1U, 0x10U);
    WriteCommandDataList(tft, 0xC5U, vcom_control, sizeof(vcom_control));
    WriteCommandData(tft, 0xC7U, 0x86U);
    WriteCommandData(tft, TFT_CMD_PIXFMT, 0x55U);
    WriteCommandDataList(tft, 0xB1U, frame_control, sizeof(frame_control));
    WriteCommandDataList(tft, 0xB6U, display_function, sizeof(display_function));
    WriteCommandData(tft, 0xF2U, 0x00U);
    WriteCommandData(tft, TFT_CMD_GAMMASET, 0x01U);
    WriteCommandDataList(tft, 0xE0U, positive_gamma, sizeof(positive_gamma));
    WriteCommandDataList(tft, 0xE1U, negative_gamma, sizeof(negative_gamma));
}

static void SetAddressWindow(spfd5408_tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    WriteCommand(tft, TFT_CMD_CASET);
    WriteData16(tft, x0);
    WriteData16(tft, x1);

    WriteCommand(tft, TFT_CMD_PASET);
    WriteData16(tft, y0);
    WriteData16(tft, y1);

    WriteCommand(tft, TFT_CMD_RAMWR);
}

bool SPFD5408_Init(spfd5408_tft_t *tft, const spfd5408_tft_config_t *config)
{
    if ((tft == NULL) || (config == NULL) || (config->write_command == NULL) ||
        (config->write_data == NULL) || (config->reset == NULL) || (config->delay_ms == NULL))
    {
        return false;
    }

    tft->config = *config;
    tft->width = SPFD5408_TFT_WIDTH;
    tft->height = SPFD5408_TFT_HEIGHT;
    tft->rotation = 0U;

    tft->config.reset(tft->config.context);
    WriteCommand(tft, TFT_CMD_SWRESET);
    tft->config.delay_ms(tft->config.context, 150U);

    RunIli9341Init(tft);
    SPFD5408_SetRotation(tft, 0U);

    WriteCommand(tft, TFT_CMD_SLPOUT);
    tft->config.delay_ms(tft->config.context, 150U);

    WriteCommand(tft, TFT_CMD_DISPON);
    tft->config.delay_ms(tft->config.context, 120U);
    SPFD5408_FillScreen(tft, SPFD5408_COLOR_BLACK);
    return true;
}

void SPFD5408_SetRotation(spfd5408_tft_t *tft, uint8_t rotation)
{
    static const uint8_t madctl[] = {
        TFT_MADCTL_MX | TFT_MADCTL_BGR,
        TFT_MADCTL_MV | TFT_MADCTL_BGR,
        TFT_MADCTL_MY | TFT_MADCTL_BGR,
        TFT_MADCTL_MX | TFT_MADCTL_MY | TFT_MADCTL_MV | TFT_MADCTL_BGR,
    };

    if (tft == NULL)
    {
        return;
    }

    tft->rotation = rotation & 3U;
    if ((tft->rotation == 0U) || (tft->rotation == 2U))
    {
        tft->width = SPFD5408_TFT_WIDTH;
        tft->height = SPFD5408_TFT_HEIGHT;
    }
    else
    {
        tft->width = SPFD5408_TFT_HEIGHT;
        tft->height = SPFD5408_TFT_WIDTH;
    }

    WriteCommandData(tft, TFT_CMD_MADCTL, madctl[tft->rotation]);
}

void SPFD5408_FillScreen(spfd5408_tft_t *tft, uint16_t color)
{
    if (tft != NULL)
    {
        SPFD5408_FillRect(tft, 0U, 0U, tft->width, tft->height, color);
    }
}

void SPFD5408_FillRect(spfd5408_tft_t *tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint32_t count;

    if ((tft == NULL) || (x >= tft->width) || (y >= tft->height) || (width == 0U) || (height == 0U))
    {
        return;
    }

    if ((uint32_t)x + width > tft->width)
    {
        width = (uint16_t)(tft->width - x);
    }
    if ((uint32_t)y + height > tft->height)
    {
        height = (uint16_t)(tft->height - y);
    }

    SetAddressWindow(tft, x, y, (uint16_t)(x + width - 1U), (uint16_t)(y + height - 1U));
    count = (uint32_t)width * height;
    while (count > 0U)
    {
        WriteData16(tft, color);
        count--;
    }
}
