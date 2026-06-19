#include "app.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "frdmmcxa153_mfs_pins.h"
#include "peripherals.h"

#include <stdbool.h>
#include <stdint.h>

#define MFS_DISPLAY_REFRESH_MS 2U
#define MFS_BUTTON_SAMPLE_MS 5U
#define MFS_LED_TOGGLE_MS 500U
#define MFS_POT_SAMPLE_MS 200U
#define MFS_BEEP_MS 80U
#define ADC_FULL_SCALE 65535U

#ifndef MFS_BUZZER_ENABLED
#define MFS_BUZZER_ENABLED 0
#endif

static volatile bool g_mfsButtonTick;
static volatile bool g_mfsLedTick;
static volatile bool g_mfsPotTick;
static volatile uint32_t g_msTicks;
static frdmmcxa153_mfs_t g_mfsShield;

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

void SysTick_Handler(void)
{
    g_msTicks++;
    if ((g_msTicks % MFS_DISPLAY_REFRESH_MS) == 0U)
    {
        FRDMMCXA153_MFS_RefreshDisplay(&g_mfsShield);
    }
    if ((g_msTicks % MFS_BUTTON_SAMPLE_MS) == 0U)
    {
        g_mfsButtonTick = true;
    }
    if ((g_msTicks % MFS_LED_TOGGLE_MS) == 0U)
    {
        g_mfsLedTick = true;
    }
    if ((g_msTicks % MFS_POT_SAMPLE_MS) == 0U)
    {
        g_mfsPotTick = true;
    }
}

static bool take_flag(volatile bool *flag)
{
    bool value;

    __disable_irq();
    value = *flag;
    *flag = false;
    __enable_irq();

    return value;
}

static void run_mfs_demo(void)
{
    status_t status;
    uint16_t potRaw = 0U;
    uint32_t beepUntilMs = 0U;
    uint8_t ledIndex = 0U;
    uint8_t displayTest = 0U;
    bool previousButtonState[MFS_BUTTON_COUNT] = {false, false, false};

    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    status = FRDMMCXA153_MFS_Init(&g_mfsShield);

    if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
    {
        PRINTF("SysTick init failed\r\n");
    }

    PRINTF("FRDM-MCXA153 Arduino Multi-Function Shield demo\r\n");
    PRINTF("Shield notes: use 3.3 V-safe I/O or level shifting; keep A0 within the ADC reference range.\r\n");
    PRINTF("Diagnostics: S1/S2/S3 states print on changes; buzzer is %s.\r\n",
           MFS_BUZZER_ENABLED ? "enabled" : "disabled");
    if (status != kStatus_Success)
    {
        PRINTF("Shield init returned status %d; demo continues without blocking forever\r\n", (int)status);
    }

    MFS_DisplaySetNumber(&g_mfsShield.display, 153);
    MFS_BuzzerSet(&g_mfsShield.buzzer, false);

    while (1)
    {
        bool buttonTick = take_flag(&g_mfsButtonTick);
        bool ledTick = take_flag(&g_mfsLedTick);
        bool potTick = take_flag(&g_mfsPotTick);

        if (buttonTick)
        {
            FRDMMCXA153_MFS_UpdateButtons(&g_mfsShield);
            for (uint8_t i = 0U; i < MFS_BUTTON_COUNT; i++)
            {
                bool pressed = MFS_ButtonsIsPressed(&g_mfsShield.buttons, i);
                if (pressed != previousButtonState[i])
                {
                    previousButtonState[i] = pressed;
                    PRINTF("S%d %s\r\n", (int)(i + 1U), pressed ? "down" : "up");
                }
                if (MFS_ButtonsTakePressed(&g_mfsShield.buttons, i))
                {
#if MFS_BUZZER_ENABLED
                    beepUntilMs = g_msTicks + MFS_BEEP_MS;
#endif
                    PRINTF("S%d pressed\r\n", (int)(i + 1U));
                }
            }
        }

        if (ledTick)
        {
            for (uint8_t i = 0U; i < MFS_LED_COUNT; i++)
            {
                MFS_LedSet(&g_mfsShield.leds, i, i == ledIndex);
            }
            ledIndex = (uint8_t)((ledIndex + 1U) % MFS_LED_COUNT);

            {
                uint8_t patterns[MFS_DISPLAY_DIGITS] = {0U, 0U, 0U, 0U};
                bool enabled[MFS_DISPLAY_DIGITS] = {true, true, true, true};
                patterns[displayTest % MFS_DISPLAY_DIGITS] = 0xFFU;
                MFS_DisplaySetRaw(&g_mfsShield.display, patterns, enabled);
                displayTest++;
            }
        }

        if (potTick)
        {
            status = FRDMMCXA153_MFS_ReadPotRaw(&potRaw);
            if (status == kStatus_Success)
            {
                PRINTF("A0 pot raw=%u buttons=%u%u%u\r\n", potRaw,
                       MFS_ButtonsIsPressed(&g_mfsShield.buttons, 0U) ? 1U : 0U,
                       MFS_ButtonsIsPressed(&g_mfsShield.buttons, 1U) ? 1U : 0U,
                       MFS_ButtonsIsPressed(&g_mfsShield.buttons, 2U) ? 1U : 0U);
            }
            else
            {
                PRINTF("A0 read failed status=%d\r\n", (int)status);
            }
        }

#if MFS_BUZZER_ENABLED
        MFS_BuzzerSet(&g_mfsShield.buzzer, (beepUntilMs != 0U) && (g_msTicks < beepUntilMs));
#else
        (void)beepUntilMs;
        MFS_BuzzerSet(&g_mfsShield.buzzer, false);
#endif
        __WFI();
    }
}

int main(void)
{
    run_mfs_demo();
}
