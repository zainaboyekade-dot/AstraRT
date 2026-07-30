#ifndef ASTRA_UART_H
#define ASTRA_UART_H

#include <stdint.h>

void UART_Send(const uint8_t *data, uint16_t size);

uint8_t UART_Receive(uint8_t *data);

#endif
