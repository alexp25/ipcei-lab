#include "mfs_pot.h"

#include "fsl_clock.h"
#include "fsl_lpadc.h"
#include "fsl_reset.h"

#include <stddef.h>

#define MFS_POT_ADC_BASE               ADC0
#define MFS_POT_ADC_COMMAND_ID         1U
#define MFS_POT_ADC_TRIGGER_ID         0U
#define MFS_POT_ADC_TRIGGER_MASK       (1UL << MFS_POT_ADC_TRIGGER_ID)
#define MFS_POT_ADC_READ_TIMEOUT_LOOPS 100000U

static mfs_adc_channel_t s_channel;
static bool s_initialized;

status_t MFS_PotInit(mfs_adc_channel_t channel)
{
    lpadc_config_t adcConfig;
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_trigger_config_t triggerConfig;

    s_channel = channel;

    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
    CLOCK_AttachClk(kFRO12M_to_ADC0);
    RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);

    LPADC_GetDefaultConfig(&adcConfig);
    adcConfig.powerLevelMode = kLPADC_PowerLevelAlt4;
    adcConfig.enableAnalogPreliminary = true;
    adcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    LPADC_Init(MFS_POT_ADC_BASE, &adcConfig);

    (void)LPADC_DoOffsetCalibration(MFS_POT_ADC_BASE);
    (void)LPADC_DoAutoCalibration(MFS_POT_ADC_BASE);

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = s_channel.channel;
    commandConfig.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(MFS_POT_ADC_BASE, MFS_POT_ADC_COMMAND_ID, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = MFS_POT_ADC_COMMAND_ID;
    triggerConfig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(MFS_POT_ADC_BASE, MFS_POT_ADC_TRIGGER_ID, &triggerConfig);

    s_initialized = true;
    return kStatus_Success;
}

status_t MFS_PotReadRaw(uint16_t *rawValue)
{
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_result_t result;
    uint32_t timeout = MFS_POT_ADC_READ_TIMEOUT_LOOPS;

    if (rawValue == NULL)
    {
        return kStatus_InvalidArgument;
    }
    if (!s_initialized)
    {
        return kStatus_Fail;
    }

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = s_channel.channel;
    commandConfig.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(MFS_POT_ADC_BASE, MFS_POT_ADC_COMMAND_ID, &commandConfig);

    LPADC_DoSoftwareTrigger(MFS_POT_ADC_BASE, MFS_POT_ADC_TRIGGER_MASK);
    while (!LPADC_GetConvResult(MFS_POT_ADC_BASE, &result))
    {
        if (--timeout == 0U)
        {
            return kStatus_Timeout;
        }
    }

    *rawValue = result.convValue;
    return kStatus_Success;
}
