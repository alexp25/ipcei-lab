#include "dfrobot_lcd_shield_keypad.h"

#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_lpadc.h"
#include "fsl_reset.h"

#include <stddef.h>

#define KEYPAD_ADC_BASE            ADC0
#define KEYPAD_ADC_CHANNEL         8U
#define KEYPAD_ADC_COMMAND_ID      1U
#define KEYPAD_ADC_TRIGGER_ID      0U
#define KEYPAD_ADC_TRIGGER         (1UL << KEYPAD_ADC_TRIGGER_ID)
#define KEYPAD_ADC_TIMEOUT         100000U
#define KEYPAD_STABLE_SAMPLE_COUNT 3U

/* Raw 16-bit LPADC thresholds from measured shield values:
 * RIGHT ~= 15, UP ~= 19500, DOWN ~= 38300, LEFT ~= 58500.
 * SELECT is expected above LEFT and below the open-circuit NONE value.
 */
static const dfrobot_lcd_shield_keypad_thresholds_t s_keypadThresholds = {
    .right_max = 9750U,
    .up_max = 28900U,
    .down_max = 48400U,
    .left_max = 62000U,
    .select_max = 65000U,
};

static bool KeypadReadAdc(uint16_t *value)
{
    lpadc_conv_result_t result;
    uint32_t timeout = KEYPAD_ADC_TIMEOUT;

    LPADC_DoSoftwareTrigger(KEYPAD_ADC_BASE, KEYPAD_ADC_TRIGGER);
    while (timeout > 0U)
    {
        if (LPADC_GetConvResult(KEYPAD_ADC_BASE, &result))
        {
            *value = result.convValue;
            return true;
        }
        --timeout;
    }

    return false;
}

static dfrobot_lcd_shield_button_t KeypadDecode(const dfrobot_lcd_shield_keypad_t *keypad, uint16_t raw_value)
{
    if (raw_value <= keypad->thresholds.right_max)
    {
        return kDFRobotLcdShieldButtonRight;
    }
    if (raw_value <= keypad->thresholds.up_max)
    {
        return kDFRobotLcdShieldButtonUp;
    }
    if (raw_value <= keypad->thresholds.down_max)
    {
        return kDFRobotLcdShieldButtonDown;
    }
    if (raw_value <= keypad->thresholds.left_max)
    {
        return kDFRobotLcdShieldButtonLeft;
    }
    if (raw_value <= keypad->thresholds.select_max)
    {
        return kDFRobotLcdShieldButtonSelect;
    }
    return kDFRobotLcdShieldButtonNone;
}

static void KeypadInitAdc(void)
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
    LPADC_Init(KEYPAD_ADC_BASE, &adcConfig);
    LPADC_DoOffsetCalibration(KEYPAD_ADC_BASE);
    LPADC_DoAutoCalibration(KEYPAD_ADC_BASE);

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = KEYPAD_ADC_CHANNEL;
    commandConfig.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(KEYPAD_ADC_BASE, KEYPAD_ADC_COMMAND_ID, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = KEYPAD_ADC_COMMAND_ID;
    triggerConfig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(KEYPAD_ADC_BASE, KEYPAD_ADC_TRIGGER_ID, &triggerConfig);
}

bool DFRobot_LcdShieldKeypadInit(dfrobot_lcd_shield_keypad_t *keypad)
{
    if (keypad == NULL)
    {
        return false;
    }

    KeypadInitAdc();

    keypad->thresholds = s_keypadThresholds;
    keypad->candidate = kDFRobotLcdShieldButtonNone;
    keypad->stable = kDFRobotLcdShieldButtonNone;
    keypad->candidate_count = 0U;
    keypad->stable_sample_count = KEYPAD_STABLE_SAMPLE_COUNT;
    keypad->last_raw_value = 0U;
    return true;
}

bool DFRobot_LcdShieldKeypadPoll(dfrobot_lcd_shield_keypad_t *keypad, dfrobot_lcd_shield_button_t *button)
{
    uint16_t raw_value;
    dfrobot_lcd_shield_button_t sample;

    if ((keypad == NULL) || (button == NULL) || !KeypadReadAdc(&raw_value))
    {
        return false;
    }

    keypad->last_raw_value = raw_value;
    sample = KeypadDecode(keypad, raw_value);
    if (sample != keypad->candidate)
    {
        keypad->candidate = sample;
        keypad->candidate_count = 1U;
    }
    else if (keypad->candidate_count < keypad->stable_sample_count)
    {
        keypad->candidate_count++;
    }

    if (keypad->candidate_count >= keypad->stable_sample_count)
    {
        keypad->stable = keypad->candidate;
    }

    *button = keypad->stable;
    return true;
}

uint16_t DFRobot_LcdShieldKeypadGetLastRawValue(const dfrobot_lcd_shield_keypad_t *keypad)
{
    return (keypad != NULL) ? keypad->last_raw_value : 0U;
}

const char *DFRobot_LcdShieldKeypadButtonName(dfrobot_lcd_shield_button_t button)
{
    static const char *const names[] = {
        "RIGHT",
        "UP",
        "DOWN",
        "LEFT",
        "SELECT",
        "NONE",
    };

    if ((uint32_t)button >= (sizeof(names) / sizeof(names[0])))
    {
        return "NONE";
    }
    return names[button];
}