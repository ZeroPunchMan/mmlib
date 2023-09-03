#pragma once

#include "cl_common.h"
#include "stm32f1xx_ll_gpio.h"

typedef GPIO_TypeDef *AbsGpioPort_t;
typedef uint32_t AbsGpioPin_t;
typedef uint32_t AbsGpioMode_t;
typedef uint32_t AsbGpioPull_t;

static inline void Mmhl_GpioInit(AbsGpioPort_t port, AbsGpioPin_t pin, AbsGpioMode_t mode)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(port, &GPIO_InitStruct);
}

static inline void Mmhl_GpioSetOutput(AbsGpioPort_t port, AbsGpioPin_t pin, uint8_t level)
{
    if (level)
        LL_GPIO_SetOutputPin(port, pin);
    else
        LL_GPIO_ResetOutputPin(port, pin);
}

static inline uint8_t Mmhl_GpioReadInput(AbsGpioPort_t port, AbsGpioPin_t pin)
{
    return LL_GPIO_IsInputPinSet(port, pin);
}
