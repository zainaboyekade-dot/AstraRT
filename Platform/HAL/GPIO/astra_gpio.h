#ifndef ASTRA_GPIO_H
#define ASTRA_GPIO_H

#include "stm32f4xx_hal.h"

void GPIO_WritePin(GPIO_TypeDef *GPIOx,
                   uint16_t GPIO_Pin,
                   GPIO_PinState PinState);

GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *GPIOx,
                           uint16_t GPIO_Pin);

void GPIO_TogglePin(GPIO_TypeDef *GPIOx,
                    uint16_t GPIO_Pin);

#endif
