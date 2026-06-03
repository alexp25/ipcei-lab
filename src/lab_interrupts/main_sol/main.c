
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "peripherals.h"
#include <stdio.h>

char messageBuffer[100];
uint32_t counter = 0;

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
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

static void init_sw3_interrupt(void)
{
    gpio_pin_config_t sw3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };

    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);

    const port_pin_config_t sw3_port_config = {
        .pullSelect = kPORT_PullUp,
        .pullValueSelect = kPORT_LowPullResistor,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterEnable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_LowDriveStrength,
        .driveStrength1 = kPORT_NormalDriveStrength,
        .mux = kPORT_MuxAlt0,
        .inputBuffer = kPORT_InputBufferEnable,
        .invertInput = kPORT_InputNormal,
        .lockRegister = kPORT_UnlockRegister
    };

    PORT_SetPinConfig(PORT1, BOARD_SW3_GPIO_PIN, &sw3_port_config);
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw3_config);
    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, kGPIO_InterruptFallingEdge);
    GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);

    NVIC_ClearPendingIRQ(BOARD_SW3_IRQ);
    EnableIRQ(BOARD_SW3_IRQ);
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

void BOARD_SW3_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
    {
        GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);
        toggle_leds();

        sprintf(messageBuffer, "%s pressed: toggled LEDs (%ld)\r\n", BOARD_SW3_NAME, counter);
        counter += 1;
        PRINTF("%s", messageBuffer);
    }
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
    init_sw3_interrupt();

    while (1)
    {
        __WFI();
    }
}
