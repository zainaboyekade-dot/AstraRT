#ifndef BME280_H
#define BME280_H

#include <stdint.h>
#include "stm32f4xx_hal.h"


#define BME280_ADDRESS (0x76 << 1)


HAL_StatusTypeDef BME280_Init(void);

void BME280_ReadTemperature(float *temperature);

void BME280_ReadPressure(float *pressure);

void BME280_ReadHumidity(float *humidity);


#endif
