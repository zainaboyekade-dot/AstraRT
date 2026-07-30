#ifndef ASTRA_SPI_H
#define ASTRA_SPI_H

#include "stm32f4xx_hal.h"


HAL_StatusTypeDef SPI_Transmit(uint8_t *data, uint16_t size);

HAL_StatusTypeDef SPI_Receive(uint8_t *data, uint16_t size);

#endif
