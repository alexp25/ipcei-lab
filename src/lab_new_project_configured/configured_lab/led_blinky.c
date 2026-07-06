/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include "board.h"
 #include "app.h"
 #include "peripherals.h"
 #include <stdbool.h>
 
 #include "app/uart.h"
 
 /*******************************************************************************
  * Definitions
  ******************************************************************************/
 #define APP_ADC_SW2_CHANNEL 14U
 #define APP_ADC_COMMAND_ID  1U
 #define APP_ADC_TRIGGER_ID  0U
 #define APP_ADC_TRIGGER_MASK (1UL << APP_ADC_TRIGGER_ID)
 #define APP_ADC_FULL_SCALE  65535U
 #define APP_ADC_VREF_MV     3300U
 
 /*******************************************************************************
  * Prototypes
  ******************************************************************************/
 static void PWM0_LED_InitOutput(void);
 static void PWM0_LED_UpdateFade(void);
 static void ADC0_SW2_InitConversion(void);
 static bool ADC0_SW2_ReadRaw(uint16_t *rawSample);
 static uint32_t ADC0_SW2_ConvertToMillivolts(uint16_t rawSample);
 static void LPUART0_WriteFixedVoltage(uint32_t millivolts);
 
 /*******************************************************************************
  * Variables
  ******************************************************************************/
 static volatile uint16_t counterPrintTick = 0;
 static volatile uint8_t pwmDutyCycle = 0;
 static volatile uint32_t printCounter = 0;
 static volatile bool flag = 0;
 
 /*******************************************************************************
  * Code
  ******************************************************************************/
 
 void SysTick_Handler(void)
 {
     counterPrintTick++;
     if (counterPrintTick >= 100)
     {
         counterPrintTick = 0;
         flag = true;
         GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
     }
 
     PWM0_LED_UpdateFade();
 }
 
 static void PWM0_LED_InitOutput(void)
 {
     PWM_SetupFaultDisableMap(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_faultchannel_0, 0U);
     PWM_OutputEnable(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0_A, FLEXPWM0_SM0);
     PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
 }
 
 static void PWM0_LED_UpdateFade(void)
 {
     PWM_UpdatePwmDutycycle(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_SignedCenterAligned,
                            pwmDutyCycle);
     PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
 
     pwmDutyCycle++;
     if (pwmDutyCycle >= 100)
     {
         pwmDutyCycle = 0;
     }
 }
 
 static void ADC0_SW2_InitConversion(void)
 {
     lpadc_conv_command_config_t commandConfig;
     lpadc_conv_trigger_config_t triggerConfig;
 
     LPADC_GetDefaultConvCommandConfig(&commandConfig);
     commandConfig.channelNumber = APP_ADC_SW2_CHANNEL;
     LPADC_SetConvCommandConfig(ADC0_PERIPHERAL, APP_ADC_COMMAND_ID, &commandConfig);
 
     LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
     triggerConfig.targetCommandId = APP_ADC_COMMAND_ID;
     LPADC_SetConvTriggerConfig(ADC0_PERIPHERAL, APP_ADC_TRIGGER_ID, &triggerConfig);
 }
 
 static bool ADC0_SW2_ReadRaw(uint16_t *rawSample)
 {
     lpadc_conv_result_t result;
     uint32_t timeout = 100000U;
 
     LPADC_DoSoftwareTrigger(ADC0_PERIPHERAL, APP_ADC_TRIGGER_MASK);
 
     while (!LPADC_GetConvResult(ADC0_PERIPHERAL, &result))
     {
         timeout--;
         if (timeout == 0U)
         {
             return false;
         }
     }
 
     *rawSample = (uint16_t)result.convValue;
     return true;
 }
 
 static uint32_t ADC0_SW2_ConvertToMillivolts(uint16_t rawSample)
 {
     return (((uint32_t)rawSample * APP_ADC_VREF_MV) + (APP_ADC_FULL_SCALE / 2U)) / APP_ADC_FULL_SCALE;
 }
 
 static void LPUART0_WriteFixedVoltage(uint32_t millivolts)
 {
     uint32_t fractional = millivolts % 1000U;
 
     LPUART0_WriteU32(millivolts / 1000U);
     LPUART0_WriteString(".");
     if (fractional < 100U)
     {
         LPUART0_WriteString("0");
     }
     if (fractional < 10U)
     {
         LPUART0_WriteString("0");
     }
     LPUART0_WriteU32(fractional);
     LPUART0_WriteString("V");
 }
 
 /*!
  * @brief Main function
  */
 int main(void)
 {
     /* Board pin init */
     BOARD_InitHardware();
     PWM0_LED_InitOutput();
     ADC0_SW2_InitConversion();
 
     while (1)
     {
         if (flag)
         {
             uint16_t adcRawSample;
 
             flag = 0;
             LPUART0_WriteString("counter=");
             LPUART0_WriteU32(printCounter);
             if (ADC0_SW2_ReadRaw(&adcRawSample))
             {
                 LPUART0_WriteString(" adc_raw=");
                 LPUART0_WriteU32(adcRawSample);
                 LPUART0_WriteString(" voltage=");
                 LPUART0_WriteFixedVoltage(ADC0_SW2_ConvertToMillivolts(adcRawSample));
             }
             else
             {
                 LPUART0_WriteString(" adc_error=timeout");
             }
             LPUART0_WriteString("\r\n");
             printCounter++;
         }
     }
 }
 