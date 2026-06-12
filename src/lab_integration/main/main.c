#include "app.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_lpadc.h"
#include "fsl_port.h"
#include "fsl_pwm.h"
#include "fsl_reset.h"
#include "onboard_temperature.h"
#include "peripherals.h"

#include <stdbool.h>
#include <stdint.h>

#define SAMPLE_PERIOD_MS           500U
#define PWM_FREQUENCY_HZ           1000U
#define PWM_MIN_DUTY_PERCENT       0U
#define PWM_MAX_DUTY_PERCENT       100U
#define ADC_BASE                   ADC0
#define ADC_COMMAND_ID             1U
#define ADC_TRIGGER_ID             0U
#define ADC_TRIGGER_MASK           (1UL << ADC_TRIGGER_ID)
#define ADC_THRESHOLD_CHANNEL      8U /* Arduino A0: ADC0_A8 */
#define ADC_FULL_SCALE             65535U
#define ADC_READ_TIMEOUT_LOOPS     100000U
#define DEFAULT_THRESHOLD_CELSIUS  30.0F
#define THRESHOLD_MIN_CELSIUS      20.0F
#define THRESHOLD_SPAN_CELSIUS     20.0F
#define COOLING_HYSTERESIS_CELSIUS 2.0F

typedef enum
{
    STATE_IDLE = 0,
    STATE_MONITORING,
    STATE_ALERT,
    STATE_COOLING
} app_state_t;

static volatile bool g_sampleTick;
static volatile bool g_buttonPressed;
static volatile uint32_t g_msTicks;

void LPUART0_SignalEvent(uint32_t event)
{
    (void)event;
}

void SysTick_Handler(void)
{
    g_msTicks++;
    if ((g_msTicks % SAMPLE_PERIOD_MS) == 0U)
    {
        g_sampleTick = true;
    }
}

void BOARD_SW3_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
    {
        GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);
        g_buttonPressed = true;
    }
}

static const char *state_to_string(app_state_t state)
{
    switch (state)
    {
        case STATE_IDLE:
            return "IDLE";
        case STATE_MONITORING:
            return "MONITORING";
        case STATE_ALERT:
            return "ALERT";
        case STATE_COOLING:
            return "COOLING";
        default:
            return "UNKNOWN";
    }
}

static void init_blue_led_gpio(void)
{
    gpio_pin_config_t ledConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = LOGIC_LED_OFF,
    };

    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &ledConfig);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);
}

static void init_sw3_interrupt(void)
{
    gpio_pin_config_t sw3Config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };

    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw3Config);
    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, kGPIO_InterruptFallingEdge);
    GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);

    NVIC_ClearPendingIRQ(BOARD_SW3_IRQ);
    EnableIRQ(BOARD_SW3_IRQ);
}

static void init_adc(void)
{
    lpadc_config_t adcConfig;
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_trigger_config_t triggerConfig;

    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
    CLOCK_AttachClk(kFRO12M_to_ADC0);
    RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);

    LPADC_GetDefaultConfig(&adcConfig);
    adcConfig.powerLevelMode = kLPADC_PowerLevelAlt4;
    adcConfig.enableAnalogPreliminary = true;
    adcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    LPADC_Init(ADC_BASE, &adcConfig);

    LPADC_DoOffsetCalibration(ADC_BASE);
    LPADC_DoAutoCalibration(ADC_BASE);

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = ADC_THRESHOLD_CHANNEL;
    commandConfig.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(ADC_BASE, ADC_COMMAND_ID, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = ADC_COMMAND_ID;
    triggerConfig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC_BASE, ADC_TRIGGER_ID, &triggerConfig);
}

static status_t read_adc_channel(uint32_t channel, uint16_t *rawValue)
{
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_result_t result;
    uint32_t timeout = ADC_READ_TIMEOUT_LOOPS;

    if (rawValue == NULL)
    {
        return kStatus_InvalidArgument;
    }

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = channel;
    commandConfig.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(ADC_BASE, ADC_COMMAND_ID, &commandConfig);

    LPADC_DoSoftwareTrigger(ADC_BASE, ADC_TRIGGER_MASK);
    while (!LPADC_GetConvResult(ADC_BASE, &result))
    {
        if (--timeout == 0U)
        {
            return kStatus_Timeout;
        }
    }

    *rawValue = result.convValue;
    return kStatus_Success;
}

static float adc_raw_to_threshold(uint16_t rawValue)
{
    return THRESHOLD_MIN_CELSIUS + (((float)rawValue * THRESHOLD_SPAN_CELSIUS) / (float)ADC_FULL_SCALE);
}

static void init_led_pwm(void)
{
    pwm_config_t pwmConfig;
    pwm_signal_param_t pwmSignal;

    PWM_GetDefaultConfig(&pwmConfig);
    PWM_Init(FLEXPWM0, kPWM_Module_0, &pwmConfig); /* PWM0_X0: red LED */
    PWM_Init(FLEXPWM0, kPWM_Module_1, &pwmConfig); /* PWM0_X1: green LED */

    pwmSignal.pwmChannel = kPWM_PwmX;
    pwmSignal.level = kPWM_LowTrue;
    pwmSignal.dutyCyclePercent = PWM_MIN_DUTY_PERCENT;
    pwmSignal.deadtimeValue = 0U;
    pwmSignal.faultState = kPWM_PwmFaultState0;
    pwmSignal.pwmchannelenable = true;

    (void)PWM_SetupPwm(FLEXPWM0, kPWM_Module_0, &pwmSignal, 1U, kPWM_EdgeAligned, PWM_FREQUENCY_HZ, SystemCoreClock);
    (void)PWM_SetupPwm(FLEXPWM0, kPWM_Module_1, &pwmSignal, 1U, kPWM_EdgeAligned, PWM_FREQUENCY_HZ, SystemCoreClock);

    PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_faultchannel_0, 0U);
    PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0, kPWM_PwmX, kPWM_Module_0);
    PWM_OutputEnable(FLEXPWM0, kPWM_PwmX, kPWM_Module_1);
    PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
    PWM_StartTimer(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1);
}

static void set_led_pwm(uint8_t redDutyPercent, uint8_t greenDutyPercent)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_EdgeAligned, redDutyPercent);
    PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_EdgeAligned, greenDutyPercent);
    PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
}

static uint8_t temperature_to_duty(float temperatureCelsius, float thresholdCelsius)
{
    float span = thresholdCelsius - THRESHOLD_MIN_CELSIUS;
    float scaled;

    if (span < 1.0F)
    {
        span = 1.0F;
    }

    scaled = ((temperatureCelsius - THRESHOLD_MIN_CELSIUS) * 100.0F) / span;
    if (scaled < 0.0F)
    {
        scaled = 0.0F;
    }
    if (scaled > 100.0F)
    {
        scaled = 100.0F;
    }

    return (uint8_t)scaled;
}

static void process_fsm(app_state_t *state, bool buttonPressed, bool temperatureValid, float temperatureCelsius,
                        float thresholdCelsius)
{
    if (buttonPressed)
    {
        *state = (*state == STATE_IDLE) ? STATE_MONITORING : STATE_IDLE;
        return;
    }

    if (!temperatureValid || (*state == STATE_IDLE))
    {
        return;
    }

    switch (*state)
    {
        case STATE_MONITORING:
            if (temperatureCelsius >= thresholdCelsius)
            {
                *state = STATE_ALERT;
            }
            break;
        case STATE_ALERT:
            if (temperatureCelsius < (thresholdCelsius - COOLING_HYSTERESIS_CELSIUS))
            {
                *state = STATE_COOLING;
            }
            break;
        case STATE_COOLING:
            if (temperatureCelsius < (thresholdCelsius - COOLING_HYSTERESIS_CELSIUS))
            {
                *state = STATE_MONITORING;
            }
            else if (temperatureCelsius >= thresholdCelsius)
            {
                *state = STATE_ALERT;
            }
            break;
        default:
            *state = STATE_IDLE;
            break;
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

int main(void)
{
    status_t temperatureStatus;
    app_state_t state = STATE_IDLE;
    float latestTemperature = 0.0F;
    float thresholdCelsius = DEFAULT_THRESHOLD_CELSIUS;
    uint16_t adcRaw = 0U;
    bool temperatureValid = false;

    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    init_blue_led_gpio();
    init_sw3_interrupt();
    init_adc();
    init_led_pwm();

    if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
    {
        PRINTF("SysTick init failed\r\n");
    }

    PRINTF("FRDM-MCXA153 integration demo: GPIO + UART + IRQ + SysTick/PWM + ADC + LPI2C\r\n");
    PRINTF("Press SW3 to start/stop monitoring. A0 sets threshold from %.1f C to %.1f C.\r\n",
           (double)THRESHOLD_MIN_CELSIUS,
           (double)(THRESHOLD_MIN_CELSIUS + THRESHOLD_SPAN_CELSIUS));

    temperatureStatus = onboard_temperature_init();
    if (temperatureStatus != kStatus_Success)
    {
        PRINTF("P3T1755 init failed (status %d); app continues without temperature updates\r\n", (int)temperatureStatus);
    }

    while (1)
    {
        bool sampleTick = take_flag(&g_sampleTick);
        bool buttonPressed = take_flag(&g_buttonPressed);

        if (sampleTick)
        {
            status_t adcStatus = read_adc_channel(ADC_THRESHOLD_CHANNEL, &adcRaw);
            if (adcStatus == kStatus_Success)
            {
                thresholdCelsius = adc_raw_to_threshold(adcRaw);
            }

            if (temperatureStatus == kStatus_Success)
            {
                temperatureStatus = onboard_temperature_read_celsius(&latestTemperature);
                temperatureValid = (temperatureStatus == kStatus_Success);
            }
            else
            {
                temperatureValid = false;
            }

            process_fsm(&state, buttonPressed, temperatureValid, latestTemperature, thresholdCelsius);

            if (state == STATE_ALERT)
            {
                set_led_pwm(PWM_MAX_DUTY_PERCENT, PWM_MIN_DUTY_PERCENT);
                GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);
            }
            else if ((state == STATE_MONITORING) || (state == STATE_COOLING))
            {
                uint8_t redDuty = temperatureValid ? temperature_to_duty(latestTemperature, thresholdCelsius) : 0U;
                set_led_pwm(redDuty, (uint8_t)(PWM_MAX_DUTY_PERCENT - redDuty));
                GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN,
                              (state == STATE_COOLING) ? LOGIC_LED_ON : LOGIC_LED_OFF);
            }
            else
            {
                set_led_pwm(PWM_MIN_DUTY_PERCENT, PWM_MIN_DUTY_PERCENT);
                GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);
            }

            if (temperatureValid)
            {
                PRINTF("state=%s temp=%.2f C threshold=%.2f C adc=%u\r\n", state_to_string(state),
                       (double)latestTemperature, (double)thresholdCelsius, adcRaw);
            }
            else
            {
                PRINTF("state=%s temp=N/A status=%d threshold=%.2f C adc=%u\r\n", state_to_string(state),
                       (int)temperatureStatus, (double)thresholdCelsius, adcRaw);
            }
        }
        else if (buttonPressed)
        {
            process_fsm(&state, true, temperatureValid, latestTemperature, thresholdCelsius);
            PRINTF("SW3 pressed: state=%s\r\n", state_to_string(state));
        }

        __WFI();
    }
}