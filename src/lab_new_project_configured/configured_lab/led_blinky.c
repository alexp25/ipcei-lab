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
 #define APP_P3T1755_I2C_ADDRESS 0x48U
 #define APP_P3T1755_TEMP_REG    0x00U
 #define APP_P3T1755_TEMP_BYTES  2U
 
 /*******************************************************************************
  * Prototypes
  ******************************************************************************/
 static void PWM0_LED_InitOutput(void);
 static void PWM0_LED_UpdateFade(void);
 static void ADC0_SW2_InitConversion(void);
 static bool ADC0_SW2_ReadRaw(uint16_t *rawSample);
 static uint32_t ADC0_SW2_ConvertToMillivolts(uint16_t rawSample);
 static bool P3T1755_ReadRaw(int16_t *rawTemperature);
 static int32_t P3T1755_RawToMilliCelsius(int16_t rawTemperature);
 static void LPUART0_WriteFixedVoltage(uint32_t millivolts);
 static void LPUART0_WriteSignedFixedTemperature(int32_t milliCelsius);
 
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

 static bool P3T1755_ReadRaw(int16_t *rawTemperature)
 {
     uint8_t rxBuffer[APP_P3T1755_TEMP_BYTES];
     lpi2c_master_transfer_t transfer = {0};
     status_t status;
     uint16_t sample;
     int16_t raw;

     transfer.flags = kLPI2C_TransferDefaultFlag;
     transfer.slaveAddress = APP_P3T1755_I2C_ADDRESS;
     transfer.direction = kLPI2C_Read;
     transfer.subaddress = APP_P3T1755_TEMP_REG;
     transfer.subaddressSize = 1U;
     transfer.data = rxBuffer;
     transfer.dataSize = sizeof(rxBuffer);

     status = LPI2C_MasterTransferBlocking(LPI2C0_PERIPHERAL, &transfer);
     if (status != kStatus_Success)
     {
         return false;
     }

     sample = ((uint16_t)rxBuffer[0] << 8U) | rxBuffer[1];
     raw = (int16_t)(sample >> 5U);
     if ((raw & 0x0400) != 0)
     {
         raw |= (int16_t)0xF800;
     }

     *rawTemperature = raw;
     return true;
 }

 static int32_t P3T1755_RawToMilliCelsius(int16_t rawTemperature)
 {
     return (int32_t)rawTemperature * 125;
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

 static void LPUART0_WriteSignedFixedTemperature(int32_t milliCelsius)
 {
     uint32_t magnitude;
     uint32_t fractional;

     if (milliCelsius < 0)
     {
         LPUART0_WriteString("-");
         magnitude = (uint32_t)(-milliCelsius);
     }
     else
     {
         magnitude = (uint32_t)milliCelsius;
     }

     fractional = magnitude % 1000U;
     LPUART0_WriteU32(magnitude / 1000U);
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
     LPUART0_WriteString("C");
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
             int16_t temperatureRaw;
 
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
             LPUART0_WriteString(" temp=");
             if (P3T1755_ReadRaw(&temperatureRaw))
             {
                 LPUART0_WriteSignedFixedTemperature(P3T1755_RawToMilliCelsius(temperatureRaw));
             }
             else
             {
                 LPUART0_WriteString("i2c_error");
             }
             LPUART0_WriteString("\r\n");
             printCounter++;
         }
     }
 }
 
