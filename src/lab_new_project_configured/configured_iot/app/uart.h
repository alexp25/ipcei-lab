
#ifndef APP_UART_H_
#define APP_UART_H_

#include <stdbool.h>
#include <stdint.h>

void LPUART2_WriteString(const char *text);
void LPUART2_WriteU32(uint32_t value);
void LPUART2_WriteSignedQ4Hundredths(int16_t valueQ4);
void LPUART2_WriteTemperatureDataJson(int16_t temperatureQ4);
void LPUART0_WriteTemperatureDataJson(int16_t temperatureQ4);

void LPUART0_WriteByte(uint8_t data);
void LPUART2_WriteByte(uint8_t data);
bool LPUART0_TryReadByte(uint8_t *data);
bool LPUART2_TryReadByte(uint8_t *data);

#endif /* APP_UART_H_ */
