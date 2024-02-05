//
// Created by vm on 24.14.1.
//

#include "Hw.h"
#include "main.h"
#include "Drivers/largeFont.h"
#include <memory>

void Hw::start(ADC_HandleTypeDef &adc,
               I2C_HandleTypeDef &i2c,
               TIM_HandleTypeDef &htim2,
               TIM_HandleTypeDef &htim3,
               TIM_HandleTypeDef &htim4,
               TIM_HandleTypeDef &htim10) {
    Hw::delayTimer = std::make_unique<TIM_HandleTypeDef>(htim10);
    Hw::_encoder1 = std::make_unique<TIM_HandleTypeDef>(htim4);

    HAL_TIM_Base_Start(Hw::delayTimer.get());
    HAL_TIM_Encoder_Start(Hw::_encoder1.get(), TIM_CHANNEL_ALL);

    Hw::lcd = std::make_unique<Lcd>(i2c, 0x3F);
}

void Hw::init() {
    Hw::lcd->init();
    Hw::ready = true;
}

void Hw::pinWrite(Pin pin, State state) {
    HAL_GPIO_WritePin(pin.GPIO, pin.GPIO_Pin, (GPIO_PinState) state);
}

void Hw::delayUs(uint16_t us) {
    //__HAL_TIM_SET_COUNTER(Hw::delayTimer, 0);
    Hw::delayTimer->Instance->CNT = 0;
    // wait for the counter to reach the us input in the parameter
    while (((Hw::delayTimer)->Instance->CNT) < us);
}

std::shared_ptr<Encoder> Hw::getEncoder1() {
    return Hw::encoder1;
}

bool Hw::isReady() {
    return Hw::ready;
}

void Hw::setVoltage(float voltage) {
    if (voltage < 0) {
        voltage = 0;
    }
    _setVoltage = voltage;
}

float Hw::getRealVoltage() {
    return _realVoltage;
}

float Hw::getRealCurrent() {
    return 0;
}

float Hw::getSetVoltage() {
    return _setVoltage;
}

float Hw::getSetCurrent() {
    return 0;
}

void Hw::sampleHardware() {
    //buttons
    for (const auto &button: Hw::buttons) {

        State state = pinRead(button->pin);

        if (button->state != state) {
            button->hasChanged = true;

            if (button->state == LOW && state == HIGH) {
                button->risingEdge = true;
                button->fallingEdge = false;
            } else {
                button->risingEdge = false;
                button->fallingEdge = true;
            }
        } else {
            button->hasChanged = false;
            button->risingEdge = false;
            button->fallingEdge = false;
        }


        button->state = state;
    }

    //encoders
    uint32_t enc1Val = _encoder1->Instance->CNT >> 2;
    if (Hw::encoder1->value != enc1Val) {

        int32_t rawdiff = Hw::encoder1->value - enc1Val;

        // overflow
        if (rawdiff > 50 || rawdiff < -50) {
            rawdiff = 1;
        }
        // slippage
        if (rawdiff > 20) {
            rawdiff = 20;
        } else if (rawdiff < -20) {
            rawdiff = -20;
        }

        Hw::encoder1->hasChanged = true;
        Hw::encoder1->diff = rawdiff;
        Hw::encoder1->value = (_encoder1->Instance->CNT >> 2);
    } else {
        Hw::encoder1->hasChanged = false;
    }
}

State Hw::pinRead(Pin pin) {
    return static_cast<State>(HAL_GPIO_ReadPin(pin.GPIO, pin.GPIO_Pin));
}

