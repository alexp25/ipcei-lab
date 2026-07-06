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
 
 /*******************************************************************************
  * Prototypes
  ******************************************************************************/
 static void PWM0_LED_InitOutput(void);
 static void PWM0_LED_UpdateFade(void);
 
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
 
 /*!
  * @brief Main function
  */
 int main(void)
 {
     /* Board pin init */
     BOARD_InitHardware();
     PWM0_LED_InitOutput();
 
     while (1)
     {
         if (flag)
         {
             flag = 0;
             LPUART0_WriteString("counter=");
             LPUART0_WriteU32(printCounter);
             LPUART0_WriteString("\r\n");
             printCounter++;
         }
     }
 }
 