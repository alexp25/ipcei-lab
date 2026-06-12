#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "onboard_temperature.h"
#include "peripherals.h"

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

int main(void)
{
    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    PRINTF("Hello from FRDM-MCXA153!\r\n");

    status_t status = onboard_temperature_init();

    if (status != kStatus_Success)
    {
        PRINTF("P3T1755 init failed (status %d)\r\n", (int)status);
    }

    while (1)
    {
        if (status == kStatus_Success)
        {
            float temperatureCelsius;
            status_t readStatus = onboard_temperature_read_celsius(&temperatureCelsius);

            if (readStatus == kStatus_Success)
            {
                PRINTF("P3T1755 temperature: %.2f C\r\n", (double)temperatureCelsius);
            }
            else
            {
                PRINTF("P3T1755 read failed (status %d)\r\n", (int)readStatus);
            }
        }

        SDK_DelayAtLeastUs(500000U, SystemCoreClock);
    }
}
