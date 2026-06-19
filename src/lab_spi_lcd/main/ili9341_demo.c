#include "ili9341_demo.h"
#include "board_ili9341.h"
#include "fsl_debug_console.h"

status_t ILI9341_DemoRun(void)
{
    ili9341_t lcd;
    ili9341_config_t config;

    BOARD_InitIli9341Pins();
    BOARD_GetIli9341Config(&config);

    status_t status = ili9341_init(&lcd, &config);
    if (status != kStatus_Success)
    {
        PRINTF("ILI9341 init failed: %ld\r\n", (long)status);
        return status;
    }

    status = ili9341_set_rotation(&lcd, 1U);
    if (status != kStatus_Success)
    {
        return status;
    }

    ili9341_fill_screen(&lcd, ILI9341_COLOR_BLACK);
    ili9341_write_string(&lcd, 32U, 24U, "FRDM-MCXA153", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK, 3U);
    ili9341_write_string(&lcd, 34U, 58U, "ILI9341 SPI Demo", ILI9341_COLOR_CYAN, ILI9341_COLOR_BLACK, 2U);

    ili9341_fill_rect(&lcd, 20U, 100U, 45U, 60U, ILI9341_COLOR_RED);
    ili9341_fill_rect(&lcd, 70U, 100U, 45U, 60U, ILI9341_COLOR_GREEN);
    ili9341_fill_rect(&lcd, 120U, 100U, 45U, 60U, ILI9341_COLOR_BLUE);
    ili9341_fill_rect(&lcd, 170U, 100U, 45U, 60U, ILI9341_COLOR_YELLOW);
    ili9341_fill_rect(&lcd, 220U, 100U, 45U, 60U, ILI9341_COLOR_MAGENTA);

    ili9341_draw_line(&lcd, 0, 0, (int16_t)(lcd.width - 1U), (int16_t)(lcd.height - 1U), ILI9341_COLOR_ORANGE);
    ili9341_draw_line(&lcd, 0, (int16_t)(lcd.height - 1U), (int16_t)(lcd.width - 1U), 0, ILI9341_COLOR_CYAN);
    ili9341_draw_line(&lcd, 0, 185, (int16_t)(lcd.width - 1U), 185, ILI9341_COLOR_WHITE);
    ili9341_write_string(&lcd, 24U, 198U, "RGB565 TEXT + LINES", ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK, 2U);

    PRINTF("ILI9341 demo drawn\r\n");
    return kStatus_Success;
}
