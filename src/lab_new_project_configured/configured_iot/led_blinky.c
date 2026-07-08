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
#include <stddef.h>

#include "app/uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC_SW2_CHANNEL 14U
#define ADC_COMMAND_ID 1U
#define ADC_TRIGGER_ID 0U
#define ADC_PRINT_TICKS 100U
#define PWM_FADE_DUTY_MAX 100U
#define P3T1755_I2C_ADDRESS 0x48U
#define P3T1755_TEMP_REG 0x00U
#define P3T1755_TEMP_RX_SIZE 2U
#define UART0_CHAT_BUFFER_SIZE 96U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC0_SW2_InitCommandTrigger(void);
static uint32_t ADC0_SW2_GetSample(void);
static void PWM0_LED_InitOutput(void);
static void PWM0_LED_UpdateFade(void);
static status_t P3T1755_ReadTemperatureRaw(int16_t *temperatureQ4);
static void UART_WriteU32(void (*writeString)(const char *text), uint32_t value);
static void UART_WriteSignedQ4Hundredths(void (*writeString)(const char *text), int16_t valueQ4);
static void UART_WriteTemperatureDataJson(void (*writeString)(const char *text), int16_t temperatureQ4);
static void UART_WriteEscapedJsonString(void (*writeString)(const char *text), const char *text);
static void UART_WriteChatJson(void (*writeString)(const char *text), const char *message);
static void UART0_ProcessConsoleByte(uint8_t data);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint16_t counterPrintTick = 0;
static volatile uint32_t printCounter = 0;
static volatile bool flag = false;
static volatile uint8_t pwmDutyCycle = 0;
static char uart0ChatBuffer[UART0_CHAT_BUFFER_SIZE];
static size_t uart0ChatLength = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    counterPrintTick++;
    if (counterPrintTick >= ADC_PRINT_TICKS)
    {
        counterPrintTick = 0;
        flag = true;
        GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);        
    }

    PWM0_LED_UpdateFade();
}

static void ADC0_SW2_InitCommandTrigger(void)
{
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_trigger_config_t triggerConfig;

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = ADC_SW2_CHANNEL;
    LPADC_SetConvCommandConfig(ADC0_PERIPHERAL, ADC_COMMAND_ID, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = ADC_COMMAND_ID;
    LPADC_SetConvTriggerConfig(ADC0_PERIPHERAL, ADC_TRIGGER_ID, &triggerConfig);
}

static uint32_t ADC0_SW2_GetSample(void)
{
    lpadc_conv_result_t result;
    LPADC_DoSoftwareTrigger(ADC0_PERIPHERAL, 1UL << ADC_TRIGGER_ID);
    while (!LPADC_GetConvResult(ADC0_PERIPHERAL, &result));
    return (uint32_t)result.convValue;
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
    if (pwmDutyCycle >= PWM_FADE_DUTY_MAX)
    {
        pwmDutyCycle = 0;
    }
}

static status_t P3T1755_ReadTemperatureRaw(int16_t *temperatureQ4)
{
    uint8_t rxData[P3T1755_TEMP_RX_SIZE];
    lpi2c_master_transfer_t transfer = {0};

    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = P3T1755_I2C_ADDRESS;
    transfer.direction = kLPI2C_Read;
    transfer.subaddress = P3T1755_TEMP_REG;
    transfer.subaddressSize = 1U;
    transfer.data = rxData;
    transfer.dataSize = sizeof(rxData);

    status_t status = LPI2C_MasterTransferBlocking(LPI2C0_PERIPHERAL, &transfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint16_t raw = ((uint16_t)rxData[0] << 8) | rxData[1];
    *temperatureQ4 = ((int16_t)raw) >> 4;

    return kStatus_Success;
}

static void UART_WriteU32(void (*writeString)(const char *text), uint32_t value)
{
    char buffer[11];
    size_t index = sizeof(buffer) - 1U;

    buffer[index] = '\0';

    do
    {
        index--;
        buffer[index] = (char)('0' + (value % 10U));
        value /= 10U;
    } while (value != 0U);

    writeString(&buffer[index]);
}

static void UART_WriteSignedQ4Hundredths(void (*writeString)(const char *text), int16_t valueQ4)
{
    uint32_t absoluteValue;

    if (valueQ4 < 0)
    {
        writeString("-");
        absoluteValue = (uint32_t)(-(int32_t)valueQ4);
    }
    else
    {
        absoluteValue = (uint32_t)valueQ4;
    }

    uint32_t integerPart = absoluteValue / 16U;
    uint32_t fractionalNibble = absoluteValue & 0x0FU;
    uint32_t hundredths = (fractionalNibble * 625U + 50U) / 100U;

    UART_WriteU32(writeString, integerPart);
    writeString(".");
    if (hundredths < 10U)
    {
        writeString("0");
    }
    UART_WriteU32(writeString, hundredths);
}

static void UART_WriteTemperatureDataJson(void (*writeString)(const char *text), int16_t temperatureQ4)
{
    writeString("{\"code\":\"data\",\"value\":");
    UART_WriteSignedQ4Hundredths(writeString, temperatureQ4);
    writeString("}\n");
}

static void UART_WriteEscapedJsonString(void (*writeString)(const char *text), const char *text)
{
    while (*text != '\0')
    {
        switch (*text)
        {
            case '\\':
                writeString("\\\\");
                break;
            case '"':
                writeString("\\\"");
                break;
            default:
            {
                char character[2] = {*text, '\0'};
                writeString(character);
                break;
            }
        }

        text++;
    }
}

static void UART_WriteChatJson(void (*writeString)(const char *text), const char *message)
{
    writeString("{\"code\":\"chat\",\"value\":\"");
    UART_WriteEscapedJsonString(writeString, message);
    writeString("\"}\n");
}

static void UART0_ProcessConsoleByte(uint8_t data)
{
    if (data == '\n')
    {
        uart0ChatBuffer[uart0ChatLength] = '\0';
        UART_WriteChatJson(LPUART2_WriteString, uart0ChatBuffer);
        uart0ChatLength = 0;
        return;
    }

    if (data == '\r')
    {
        return;
    }

    if (uart0ChatLength < (UART0_CHAT_BUFFER_SIZE - 1U))
    {
        uart0ChatBuffer[uart0ChatLength] = (char)data;
        uart0ChatLength++;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_InitHardware();
    ADC0_SW2_InitCommandTrigger();
    PWM0_LED_InitOutput();

    while (1)
    {
        uint8_t bridgeByte;

        if (LPUART2_TryReadByte(&bridgeByte))
        {
            LPUART0_WriteByte(bridgeByte);
        }
        if (LPUART0_TryReadByte(&bridgeByte))
        {
            UART0_ProcessConsoleByte(bridgeByte);
        }

        if (flag)
        {
            flag = false;
            uint32_t adc_value = ADC0_SW2_GetSample();
            (void)adc_value;
            int16_t temperatureQ4 = 0;
            status_t tempStatus = P3T1755_ReadTemperatureRaw(&temperatureQ4);

            if (tempStatus == kStatus_Success)
            {
                UART_WriteTemperatureDataJson(LPUART2_WriteString, temperatureQ4);
                UART_WriteTemperatureDataJson(LPUART0_WriteString, temperatureQ4);
            }
            printCounter++;
        }
    }
}
