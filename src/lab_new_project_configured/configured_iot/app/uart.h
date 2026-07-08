
#ifndef APP_UART_H_
#define APP_UART_H_

#include <stdbool.h>
#include <stdint.h>

void LPUART2_WriteString(const char *text);
void LPUART0_WriteString(const char *text);

void LPUART0_WriteByte(uint8_t data);
void LPUART2_WriteByte(uint8_t data);
bool LPUART0_TryReadByte(uint8_t *data);
bool LPUART2_TryReadByte(uint8_t *data);

#endif /* APP_UART_H_ */
