
#include "uart.h"

#include "peripherals.h"
#include <stddef.h>

static void UART_WriteString(LPUART_Type *base, const char *text)
{
    const char *end = text;

    while (*end != '\0')
    {
        end++;
    }

    (void)LPUART_WriteBlocking(base, (const uint8_t *)text, (size_t)(end - text));
}

static void UART_WriteU32(LPUART_Type *base, uint32_t value)
{
    char buffer[10];
    size_t index = sizeof(buffer);

    do
    {
        index--;
        buffer[index] = (char)('0' + (value % 10U));
        value /= 10U;
    } while (value != 0U);

    (void)LPUART_WriteBlocking(base, (const uint8_t *)&buffer[index], sizeof(buffer) - index);
}

static void UART_WriteSignedQ4Hundredths(LPUART_Type *base, int16_t valueQ4)
{
    uint32_t absoluteValue;

    if (valueQ4 < 0)
    {
        UART_WriteString(base, "-");
        absoluteValue = (uint32_t)(-(int32_t)valueQ4);
    }
    else
    {
        absoluteValue = (uint32_t)valueQ4;
    }

    uint32_t integerPart = absoluteValue / 16U;
    uint32_t fractionalNibble = absoluteValue & 0x0FU;
    uint32_t hundredths = (fractionalNibble * 625U + 50U) / 100U;

    UART_WriteU32(base, integerPart);
    UART_WriteString(base, ".");
    if (hundredths < 10U)
    {
        UART_WriteString(base, "0");
    }
    UART_WriteU32(base, hundredths);
}

static void UART_WriteTemperatureDataJson(LPUART_Type *base, int16_t temperatureQ4)
{
    UART_WriteString(base, "{\"code\":\"data\",\"value\":");
    UART_WriteSignedQ4Hundredths(base, temperatureQ4);
    UART_WriteString(base, "}\n");
}

void LPUART2_WriteString(const char *text)
{
    UART_WriteString(LPUART2_PERIPHERAL, text);
}

void LPUART2_WriteU32(uint32_t value)
{
    UART_WriteU32(LPUART2_PERIPHERAL, value);
}

void LPUART2_WriteSignedQ4Hundredths(int16_t valueQ4)
{
    UART_WriteSignedQ4Hundredths(LPUART2_PERIPHERAL, valueQ4);
}

void LPUART2_WriteTemperatureDataJson(int16_t temperatureQ4)
{
    UART_WriteTemperatureDataJson(LPUART2_PERIPHERAL, temperatureQ4);
}

void LPUART0_WriteTemperatureDataJson(int16_t temperatureQ4)
{
    UART_WriteTemperatureDataJson(LPUART0_PERIPHERAL, temperatureQ4);
}

void LPUART0_WriteByte(uint8_t data)
{
    (void)LPUART_WriteBlocking(LPUART0_PERIPHERAL, &data, 1U);
}

void LPUART2_WriteByte(uint8_t data)
{
    (void)LPUART_WriteBlocking(LPUART2_PERIPHERAL, &data, 1U);
}

bool LPUART0_TryReadByte(uint8_t *data)
{
    if ((LPUART_GetStatusFlags(LPUART0_PERIPHERAL) & (uint32_t)kLPUART_RxDataRegFullFlag) != 0U)
    {
        *data = (uint8_t)LPUART0_PERIPHERAL->DATA;
        return true;
    }

    return false;
}

bool LPUART2_TryReadByte(uint8_t *data)
{
    if ((LPUART_GetStatusFlags(LPUART2_PERIPHERAL) & (uint32_t)kLPUART_RxDataRegFullFlag) != 0U)
    {
        *data = (uint8_t)LPUART2_PERIPHERAL->DATA;
        return true;
    }

    return false;
}
