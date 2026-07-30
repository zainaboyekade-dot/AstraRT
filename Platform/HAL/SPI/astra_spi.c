/*
 * AstraRT SPI HAL
 *
 * Provides SPI communication
 * abstraction for external devices.
 */

#include "astra_spi.h"
#include "spi.h"

extern SPI_HandleTypeDef hspi2;


HAL_StatusTypeDef SPI_Transmit(uint8_t *data, uint16_t size)
{
    return HAL_SPI_Transmit(
        &hspi2,
        data,
        size,
        HAL_MAX_DELAY
    );
}


HAL_StatusTypeDef SPI_Receive(uint8_t *data, uint16_t size)
{
    return HAL_SPI_Receive(
        &hspi2,
        data,
        size,
        HAL_MAX_DELAY
    );
}
