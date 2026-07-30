/*
 * BMI160 IMU Driver
 *
 * Communication:
 * I2C
 *
 * Features:
 * - Device identification
 * - Accelerometer initialization
 * - Accelerometer data acquisition
 */

#include "bmi160.h"
#include "../../HAL/I2C/astra_i2c.h"


HAL_StatusTypeDef BMI160_ReadChipID(uint8_t *id)
{
    return I2C_ReadRegister(
        BMI160_I2C_ADDRESS,
        BMI160_CHIP_ID_REG,
        id,
        1
    );
}

HAL_StatusTypeDef BMI160_Init(void)
{
    uint8_t id;

    if(BMI160_ReadChipID(&id) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if(id != BMI160_CHIP_ID)
    {
        return HAL_ERROR;
    }


    uint8_t command = 0x11;

    if(I2C_WriteRegister(
            BMI160_I2C_ADDRESS,
            BMI160_CMD_REG,
            &command,
            1
        ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    HAL_Delay(50);

    return HAL_OK;
}


HAL_StatusTypeDef BMI160_ReadAccel(
    float *x,
    float *y,
    float *z
)
{
    uint8_t data[6];

    int16_t rawX;
    int16_t rawY;
    int16_t rawZ;


    if(I2C_ReadRegister(
            BMI160_I2C_ADDRESS,
            BMI160_ACC_X_LSB,
            data,
            6
        ) != HAL_OK)
    {
        return HAL_ERROR;
    }


    rawX = (int16_t)((data[1] << 8) | data[0]);
    rawY = (int16_t)((data[3] << 8) | data[2]);
    rawZ = (int16_t)((data[5] << 8) | data[4]);


    *x = (float)rawX;
    *y = (float)rawY;
    *z = (float)rawZ;


    return HAL_OK;
}
