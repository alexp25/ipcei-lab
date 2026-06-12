
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include <stdio.h>

#define USE_SYSTICK_FLAG 1

volatile uint8_t flag = 0;
char messageBuffer[100];
uint32_t counter = 0;

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

/**
 * internal timer interrupt handler
 */
void SysTick_Handler(void)
{
    flag = 1;
}

/**
 * init pins and self test
 */
void init_pins()
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO,
                  BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO,
                  BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);

    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON);
    SDK_DelayAtLeastUs(250000U, SystemCoreClock);
    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);

    GPIO_PinWrite(BOARD_LED_GREEN_GPIO,
                  BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_ON);
    SDK_DelayAtLeastUs(250000U, SystemCoreClock);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO,
                  BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF);

    GPIO_PinWrite(BOARD_LED_BLUE_GPIO,
                  BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_ON);
    SDK_DelayAtLeastUs(250000U, SystemCoreClock);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO,
                  BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);
}

/**
 * toggle led pins
 */
void toggle_leds()
{
    /* Toggle pin connected to LED */
    // GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);

    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
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

    init_pins();

    // configure timer clock (1/2 seconds)
    SysTick_Config(SystemCoreClock / 2U);

    while (1)
    {

#ifdef USE_SYSTICK_FLAG
        if (flag)
        {
            flag = 0;
            toggle_leds();
            sprintf(messageBuffer, "Hello from FRDM-MCXA153! (%ld)\r\n", counter);
            counter += 1;
            PRINTF("%s", messageBuffer);
        }
#else
        toggle_leds();
        sprintf(messageBuffer, "Hello from FRDM-MCXA153! (%ld)\r\n", counter);
        counter += 1;
        PRINTF("%s", messageBuffer);

        SDK_DelayAtLeastUs(500000U, SystemCoreClock);
#endif
    }
}
