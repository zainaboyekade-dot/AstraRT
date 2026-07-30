/*
 * AstraRT UART HAL
 *
 * Provides a simplified interface
 * for UART communication.
 */

#include "astra_uart.h"
#include "usart.h"

extern UART_HandleTypeDef huart2;


void UART_Send(const uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit(&huart2, data, size, HAL_MAX_DELAY);
}

uint8_t UART_Receive(uint8_t *data)
{
    return HAL_UART_Receive(&huart2, data, 1, 100);
}

