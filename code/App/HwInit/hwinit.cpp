//
// Created by vm on 24.14.1.
//
#include "hwinit.h"
#include "Hw/Hw.h"

void hwInit(ADC_HandleTypeDef *adc,
            I2C_HandleTypeDef *i2c,
            TIM_HandleTypeDef *htim2,
            TIM_HandleTypeDef *htim3,
            TIM_HandleTypeDef *htim4,
            TIM_HandleTypeDef *htim10) {
    Hw::start(*adc, *i2c, *htim2, *htim3, *htim4, *htim10);
}