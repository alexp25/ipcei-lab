
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "peripherals.h"
#include <stdbool.h>
#include <stdio.h>

char messageBuffer[100];
uint32_t counter = 0;

void on_button_pressed(void);

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

/**
 * init SW3 pin
 */
void init_sw3_gpio(void)
{
    gpio_pin_config_t sw3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U};

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
        .lockRegister = kPORT_UnlockRegister};

    PORT_SetPinConfig(PORT1, BOARD_SW3_GPIO_PIN, &sw3_port_config);
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw3_config);
}

/**
 * check if SW3 button is pressed
 */
void check_button_pressed(void)
{
    static bool was_button_pressed = false;
    bool is_button_pressed = GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN) == 0U;

    if (is_button_pressed && !was_button_pressed)
    {
        on_button_pressed();
        was_button_pressed = true;
    }
    else if (!is_button_pressed)
    {
        was_button_pressed = false;
    }
}

/**
 * check if SW3 button is pressed w/ debounce
 */
void check_button_pressed_debounce(void)
{
    static bool was_button_pressed = false;
    bool is_button_pressed = GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN) == 0U;

    if (is_button_pressed && !was_button_pressed)
    {
        SDK_DelayAtLeastUs(20000U, SystemCoreClock);
        if (is_button_pressed)
        {
            on_button_pressed();
            was_button_pressed = true;
        }
    }
    else if (!is_button_pressed)
    {
        was_button_pressed = false;
    }
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

/**
 * action on button pressed: toggle led pins + printf
 */
void on_button_pressed(void)
{
    toggle_leds();

    sprintf(messageBuffer, "%s pressed: toggled LEDs (%ld)\r\n", BOARD_SW3_NAME, counter);
    counter += 1;
    PRINTF("%s", messageBuffer);
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
    init_sw3_gpio();

    while (1)
    {
        check_button_pressed();

        // check_button_pressed_debounce();
    }
}
