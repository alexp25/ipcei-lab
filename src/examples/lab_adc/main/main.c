#include "app.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_lpadc.h"
#include "fsl_reset.h"
#include "peripherals.h"
#include <stddef.h>
#include <stdint.h>

#define ADC_BASE              ADC0
#define ADC_COMMAND_ID        1U
#define ADC_TRIGGER_ID        0U
#define ADC_TRIGGER_MASK      (1UL << ADC_TRIGGER_ID)
#define ADC_FULL_SCALE        65535U
#define ADC_REFERENCE_MV      3300U
#define ADC_SAMPLE_PERIOD_US  500000U

typedef struct
{
    const char *name;
    uint32_t channel;
} adc_input_t;

static const adc_input_t adc_inputs[] = {
    {"SW3", 23U},
    {"A0", 8U},
    {"A1", 10U},
    {"A2", 11U},
    {"A3", 0U},
    {"A4", 12U},
    {"A5", 13U},
};

static void init_leds(void)
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF);
}

static void init_adc(void)
{
    lpadc_config_t adc_config;
    lpadc_conv_command_config_t command_config;
    lpadc_conv_trigger_config_t trigger_config;

    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
    CLOCK_AttachClk(kFRO12M_to_ADC0);
    RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);

    LPADC_GetDefaultConfig(&adc_config);
    adc_config.powerLevelMode = kLPADC_PowerLevelAlt4;
    adc_config.enableAnalogPreliminary = true;
    adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    LPADC_Init(ADC_BASE, &adc_config);

    LPADC_DoOffsetCalibration(ADC_BASE);
    LPADC_DoAutoCalibration(ADC_BASE);

    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(ADC_BASE, ADC_COMMAND_ID, &command_config);

    LPADC_GetDefaultConvTriggerConfig(&trigger_config);
    trigger_config.targetCommandId = ADC_COMMAND_ID;
    trigger_config.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC_BASE, ADC_TRIGGER_ID, &trigger_config);
}

static uint16_t read_adc_channel(uint32_t channel)
{
    lpadc_conv_command_config_t command_config;
    lpadc_conv_result_t result;

    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.channelNumber = channel;
    command_config.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(ADC_BASE, ADC_COMMAND_ID, &command_config);

    LPADC_DoSoftwareTrigger(ADC_BASE, ADC_TRIGGER_MASK);
    while (!LPADC_GetConvResult(ADC_BASE, &result))
    {
    }

    return result.convValue;
}

int main(void)
{
    BOARD_InitHardware();

    CLOCK_SetClockDiv(kCLOCK_DivLPUART0, 1U);
    CLOCK_AttachClk(kFRO12M_to_LPUART0);
    BOARD_InitDebugConsole();

    init_leds();
    init_adc();

    PRINTF("ADC voltage measurement: SW3 and Arduino A0-A5\r\n");

    while (1)
    {
        for (size_t i = 0U; i < (sizeof(adc_inputs) / sizeof(adc_inputs[0])); i++)
        {
            uint16_t raw_value = read_adc_channel(adc_inputs[i].channel);
            uint32_t voltage_mv =
                ((uint32_t)raw_value * ADC_REFERENCE_MV + (ADC_FULL_SCALE / 2U)) / ADC_FULL_SCALE;

            PRINTF("%s \t (ADC0_A%u) \t raw = %u \t voltage = %u.%03u V\r\n",
                   adc_inputs[i].name,
                   adc_inputs[i].channel,
                   raw_value,
                   voltage_mv / 1000U,
                   voltage_mv % 1000U);
        }
        PRINTF("\r\n");

        SDK_DelayAtLeastUs(ADC_SAMPLE_PERIOD_US, SystemCoreClock);
    }
}
