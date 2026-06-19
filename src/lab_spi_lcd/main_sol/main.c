
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include "ili9341_demo.h"

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_InitHardware();

    BOARD_InitDebugConsole();

    PRINTF("Hello from FRDM-MCXA153!\r\n");

    status_t status = ILI9341_DemoInit();
    if (status != kStatus_Success)
    {
        PRINTF("ILI9341 demo init failed: %ld\r\n", (long)status);
        while (1)
        {
        }
    }

    uint32_t frame = 0U;

    while (1)
    {
        uint16_t screensaver_value =
            (uint16_t)((((frame * 29U) & 0x0FFFU) + (((frame * 113U) + 900U) & 0x0FFFU)) / 2U);
        ILI9341_DemoUpdate(screensaver_value);
        frame++;
        SDK_DelayAtLeastUs(12000U, CLOCK_GetCoreSysClkFreq());
    }
}
