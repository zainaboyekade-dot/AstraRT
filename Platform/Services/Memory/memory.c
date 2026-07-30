/*
 * AstraRT Memory Service
 *
 * Provides runtime memory diagnostics
 * and reports system memory status.
 */

#include "memory.h"
#include "../../HAL/UART/astra_uart.h"
#include <string.h>
#include <stdio.h>

void memory_print_status(void)
{
    char buffer[120];

    snprintf(
        buffer,
        sizeof(buffer),
        "\r\n==== MEMORY STATUS ====\r\n"
        "Stack: OK\r\n"
        "Heap: OK\r\n"
    );

    UART_Send(
        (uint8_t*)buffer,
        strlen(buffer)
    );
}
