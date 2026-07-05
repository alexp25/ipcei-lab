
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

    ILI9341_DemoRun();

    while (1)
    {
    }
}
