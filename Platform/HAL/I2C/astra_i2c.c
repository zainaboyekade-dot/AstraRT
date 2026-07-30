/*
 * AstraRT I2C HAL
 *
 * Provides hardware-independent
 * I2C communication functions
 * for sensor drivers.
 */

#include "astra_i2c.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c1;


void I2C_Init(void)
{
    MX_I2C1_Init();
}


HAL_StatusTypeDef I2C_Write(uint16_t deviceAddress,
                            uint8_t *data,
                            uint16_t size)
{
    return HAL_I2C_Master_Transmit(
        &hi2c1,
        deviceAddress,
        data,
        size,
        HAL_MAX_DELAY
    );
}


HAL_StatusTypeDef I2C_Read(uint16_t deviceAddress,
                           uint8_t *data,
                           uint16_t size)
{
    return HAL_I2C_Master_Receive(
        &hi2c1,
        deviceAddress,
        data,
        size,
        HAL_MAX_DELAY
    );
}

HAL_StatusTypeDef I2C_IsDeviceReady(uint16_t deviceAddress)
{
    return HAL_I2C_IsDeviceReady(
        &hi2c1,
        deviceAddress,
        1,
        10
    );
}

HAL_StatusTypeDef I2C_ReadRegister(
    uint16_t deviceAddress,
    uint8_t reg,
    uint8_t *data,
    uint16_t size
)
{
    return HAL_I2C_Mem_Read(
        &hi2c1,
        deviceAddress,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        size,
		HAL_MAX_DELAY
    );
}

HAL_StatusTypeDef I2C_WriteRegister(
    uint16_t deviceAddress,
    uint8_t reg,
    uint8_t *data,
    uint16_t size
)
{
    return HAL_I2C_Mem_Write(
        &hi2c1,
        deviceAddress,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        size,
		HAL_MAX_DELAY
    );
}
