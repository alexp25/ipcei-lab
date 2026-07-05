#ifndef SPFD5408_TFT_H_
#define SPFD5408_TFT_H_

#include <stdbool.h>
#include <stdint.h>

#define SPFD5408_TFT_WIDTH  240U
#define SPFD5408_TFT_HEIGHT 320U

#define SPFD5408_COLOR_BLACK   0x0000U
#define SPFD5408_COLOR_BLUE    0x001FU
#define SPFD5408_COLOR_RED     0xF800U
#define SPFD5408_COLOR_GREEN   0x07E0U
#define SPFD5408_COLOR_CYAN    0x07FFU
#define SPFD5408_COLOR_MAGENTA 0xF81FU
#define SPFD5408_COLOR_YELLOW  0xFFE0U
#define SPFD5408_COLOR_WHITE   0xFFFFU

typedef struct
{
    void *context;
    void (*write_command)(void *context, uint8_t command);
    void (*write_data)(void *context, uint8_t data);
    void (*reset)(void *context);
    void (*delay_ms)(void *context, uint32_t delay_ms);
} spfd5408_tft_config_t;

typedef struct
{
    spfd5408_tft_config_t config;
    uint16_t width;
    uint16_t height;
    uint8_t rotation;
} spfd5408_tft_t;

bool SPFD5408_Init(spfd5408_tft_t *tft, const spfd5408_tft_config_t *config);
void SPFD5408_SetRotation(spfd5408_tft_t *tft, uint8_t rotation);
void SPFD5408_FillScreen(spfd5408_tft_t *tft, uint16_t color);
void SPFD5408_FillRect(spfd5408_tft_t *tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

#endif /* SPFD5408_TFT_H_ */
