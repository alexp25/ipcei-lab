
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

void LPUART2_WriteString(const char *text)
{
    UART_WriteString(LPUART2_PERIPHERAL, text);
}

void LPUART0_WriteString(const char *text)
{
    UART_WriteString(LPUART0_PERIPHERAL, text);
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
