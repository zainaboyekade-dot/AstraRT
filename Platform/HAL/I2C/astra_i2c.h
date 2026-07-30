#ifndef ASTRA_I2C_H
#define ASTRA_I2C_H

#include "stm32f4xx_hal.h"

void I2C_Init(void);

HAL_StatusTypeDef I2C_Write(uint16_t deviceAddress, uint8_t *data, uint16_t size);

HAL_StatusTypeDef I2C_Read(uint16_t deviceAddress, uint8_t *data, uint16_t size);

HAL_StatusTypeDef I2C_IsDeviceReady(uint16_t deviceAddress);

HAL_StatusTypeDef I2C_ReadRegister(
    uint16_t deviceAddress,
    uint8_t reg,
    uint8_t *data,
    uint16_t size
);

HAL_StatusTypeDef I2C_WriteRegister(
    uint16_t deviceAddress,
    uint8_t reg,
    uint8_t *data,
    uint16_t size
);

#endif
