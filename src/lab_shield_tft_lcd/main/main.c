#include "app.h"
#include "board.h"
#include "frdmmcxa153_tft_shield.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "spfd5408_tft.h"

#include <stdint.h>

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

int main(void)
{
    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    frdmmcxa153_tft_shield_t shield;
    spfd5408_tft_t tft;

    PRINTF("FRDM-MCXA153 SPFD5408 TFT shield demo\r\n");

    if (!FRDMMCXA153_TftShieldInit(&shield) || !SPFD5408_Init(&tft, &shield.tft))
    {
        PRINTF("TFT shield initialization failed\r\n");
        while (1)
        {
            SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
        }
    }

    PRINTF("TFT initialized with write-only 0x9341/SPFD5408-compatible sequence\r\n");
    SPFD5408_SetRotation(&tft, 1U);

    while (1)
    {
        SPFD5408_FillScreen(&tft, SPFD5408_COLOR_BLACK);
        SPFD5408_FillRect(&tft, 0U, 0U, 320U, 40U, SPFD5408_COLOR_RED);
        SPFD5408_FillRect(&tft, 0U, 40U, 320U, 40U, SPFD5408_COLOR_GREEN);
        SPFD5408_FillRect(&tft, 0U, 80U, 320U, 40U, SPFD5408_COLOR_BLUE);
        SPFD5408_FillRect(&tft, 30U, 140U, 260U, 60U, SPFD5408_COLOR_WHITE);
        SDK_DelayAtLeastUs(1000000U, SystemCoreClock);

        SPFD5408_FillScreen(&tft, SPFD5408_COLOR_BLUE);
        SPFD5408_FillRect(&tft, 20U, 20U, 280U, 60U, SPFD5408_COLOR_YELLOW);
        SPFD5408_FillRect(&tft, 20U, 100U, 280U, 60U, SPFD5408_COLOR_CYAN);
        SPFD5408_FillRect(&tft, 20U, 180U, 280U, 40U, SPFD5408_COLOR_MAGENTA);
        SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
    }
}
