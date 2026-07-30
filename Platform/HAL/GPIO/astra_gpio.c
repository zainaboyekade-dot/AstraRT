/*
 * AstraRT GPIO HAL
 *
 * Provides a simplified interface
 * for GPIO operations.
 */

#include "astra_gpio.h"

void GPIO_WritePin(GPIO_TypeDef *GPIOx,
                   uint16_t GPIO_Pin,
                   GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}


GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *GPIOx,
                           uint16_t GPIO_Pin)
{
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}


void GPIO_TogglePin(GPIO_TypeDef *GPIOx,
                    uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}
