#include "frdmmcxa153_lcd_shield.h"

#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_lpadc.h"
#include "fsl_reset.h"
#include "pin_mux.h"

#include <stdint.h>

#define LCD_RS_GPIO BOARD_INITPINS_LCD_RS_GPIO
#define LCD_RS_PIN  BOARD_INITPINS_LCD_RS_PIN

#define LCD_EN_GPIO BOARD_INITPINS_LCD_EN_GPIO
#define LCD_EN_PIN  BOARD_INITPINS_LCD_EN_PIN

#define LCD_D4_GPIO BOARD_INITPINS_LCD_D4_GPIO
#define LCD_D4_PIN  BOARD_INITPINS_LCD_D4_PIN

#define LCD_D5_GPIO BOARD_INITPINS_LCD_D5_GPIO
#define LCD_D5_PIN  BOARD_INITPINS_LCD_D5_PIN

#define LCD_D6_GPIO BOARD_INITPINS_LCD_D6_GPIO
#define LCD_D6_PIN  BOARD_INITPINS_LCD_D6_PIN

#define LCD_D7_GPIO BOARD_INITPINS_LCD_D7_GPIO
#define LCD_D7_PIN  BOARD_INITPINS_LCD_D7_PIN

#define LCD_BACKLIGHT_GPIO BOARD_INITPINS_LCD_BACKLIGHT_GPIO
#define LCD_BACKLIGHT_PIN  BOARD_INITPINS_LCD_BACKLIGHT_PIN

#define KEYPAD_ADC_BASE       ADC0
#define KEYPAD_ADC_CHANNEL    8U
#define KEYPAD_ADC_COMMAND_ID 1U
#define KEYPAD_ADC_TRIGGER_ID 0U
#define KEYPAD_ADC_TRIGGER    (1UL << KEYPAD_ADC_TRIGGER_ID)
#define KEYPAD_ADC_MAX_VALUE  4095U
#define KEYPAD_ADC_SHIFT      3U
#define KEYPAD_ADC_TIMEOUT    100000U

/*
 * This shield's 5 V resistor ladder reads higher against the board's 3.3 V ADC
 * reference than the generic Arduino bands. Keep these board-specific and use
 * the serial raw/10-bit diagnostics to tune for component tolerances.
 */
static const lcd_keypad_thresholds_t s_keypadThresholds = {
    .right_max = 100U,
    .up_max = 300U,
    .down_max = 600U,
    .left_max = 850U,
    .select_max = 970U,
};

static void LcdShieldWriteRs(void *context, bool high)
{
    (void)context;
    GPIO_PinWrite(LCD_RS_GPIO, LCD_RS_PIN, high ? 1U : 0U);
}

static void LcdShieldWriteEnable(void *context, bool high)
{
    (void)context;
    GPIO_PinWrite(LCD_EN_GPIO, LCD_EN_PIN, high ? 1U : 0U);
}

static void LcdShieldWriteData(void *context, uint8_t nibble)
{
    (void)context;
    GPIO_PinWrite(LCD_D4_GPIO, LCD_D4_PIN, (nibble >> 0U) & 1U);
    GPIO_PinWrite(LCD_D5_GPIO, LCD_D5_PIN, (nibble >> 1U) & 1U);
    GPIO_PinWrite(LCD_D6_GPIO, LCD_D6_PIN, (nibble >> 2U) & 1U);
    GPIO_PinWrite(LCD_D7_GPIO, LCD_D7_PIN, (nibble >> 3U) & 1U);
}

static void LcdShieldDelay(void *context, uint32_t delay_us)
{
    (void)context;
    SDK_DelayAtLeastUs(delay_us, SystemCoreClock);
}

static bool LcdShieldReadAdc(void *context, uint16_t *value)
{
    lpadc_conv_result_t result;
    uint32_t timeout = KEYPAD_ADC_TIMEOUT;

    (void)context;
    LPADC_DoSoftwareTrigger(KEYPAD_ADC_BASE, KEYPAD_ADC_TRIGGER);
    while (timeout > 0U)
    {
        if (LPADC_GetConvResult(KEYPAD_ADC_BASE, &result))
        {
            *value = (uint16_t)(result.convValue >> KEYPAD_ADC_SHIFT);
            return true;
        }
        timeout--;
    }
    return false;
}

static void LcdShieldInitAdc(void)
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
    LPADC_Init(KEYPAD_ADC_BASE, &adc_config);

    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.channelNumber = KEYPAD_ADC_CHANNEL;
    command_config.conversionResolutionMode = kLPADC_ConversionResolutionStandard;
    LPADC_SetConvCommandConfig(KEYPAD_ADC_BASE, KEYPAD_ADC_COMMAND_ID, &command_config);

    LPADC_GetDefaultConvTriggerConfig(&trigger_config);
    trigger_config.targetCommandId = KEYPAD_ADC_COMMAND_ID;
    trigger_config.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(KEYPAD_ADC_BASE, KEYPAD_ADC_TRIGGER_ID, &trigger_config);
}

bool FRDMMCXA153_LcdShieldInit(frdmmcxa153_lcd_shield_t *shield)
{
    if (shield == NULL)
    {
        return false;
    }

    LcdShieldInitAdc();

    shield->lcd.context = NULL;
    shield->lcd.write_rs = LcdShieldWriteRs;
    shield->lcd.write_enable = LcdShieldWriteEnable;
    shield->lcd.write_data = LcdShieldWriteData;
    shield->lcd.delay_us = LcdShieldDelay;

    shield->keypad.context = NULL;
    shield->keypad.read_adc = LcdShieldReadAdc;
    shield->keypad.adc_max_value = KEYPAD_ADC_MAX_VALUE;
    shield->keypad.stable_sample_count = 3U;
    shield->keypad.thresholds_10bit = s_keypadThresholds;

    FRDMMCXA153_LcdShieldSetBacklight(true);
    return true;
}

void FRDMMCXA153_LcdShieldSetBacklight(bool on)
{
#if LCD_SHIELD_ENABLE_BACKLIGHT_CONTROL
    uint8_t level = (on == (LCD_SHIELD_BACKLIGHT_ACTIVE_HIGH != 0)) ? 1U : 0U;
    GPIO_PinWrite(LCD_BACKLIGHT_GPIO, LCD_BACKLIGHT_PIN, level);
#else
    (void)on;
#endif
}
