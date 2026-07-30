#ifndef BMI160_H
#define BMI160_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define BMI160_I2C_ADDRESS   (0x69 << 1)
#define BMI160_CHIP_ID_REG   0x00
#define BMI160_CHIP_ID       0xD1

#define BMI160_ACC_X_LSB     0x12

#define BMI160_ACC_CONFIG    0x40
#define BMI160_ACC_RANGE     0x41

#define BMI160_GYR_CONFIG    0x42
#define BMI160_GYR_RANGE     0x43

#define BMI160_CMD_REG       0x7E

HAL_StatusTypeDef BMI160_Init(void);
HAL_StatusTypeDef BMI160_ReadChipID(uint8_t *id);

HAL_StatusTypeDef BMI160_ReadAccel(
    float *x,
    float *y,
    float *z
);


#endif
