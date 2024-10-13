//
// Created by vm on 24.14.1.
//

#include "Hw.h"
#include "main.h"
#include "Drivers/largeFont.h"
#include "cmsis_os2.h"
#include <memory>

void Hw::start(ADC_HandleTypeDef &adc,
               I2C_HandleTypeDef &i2c,
               TIM_HandleTypeDef &htim2,
               TIM_HandleTypeDef &htim3,
               TIM_HandleTypeDef &htim4,
               TIM_HandleTypeDef &htim10) {
    // htim2;  2 chanel pwm
    // htim3;  encoder 1
    // htim4;  encoder 2
    // htim10; for micro delays


    Hw::_encoder2 = std::make_unique<TIM_HandleTypeDef>(htim3);
    Hw::_encoder1 = std::make_unique<TIM_HandleTypeDef>(htim4);
    Hw::pwm = std::make_unique<TIM_HandleTypeDef>(htim2);
    Hw::delayTimer = std::make_unique<TIM_HandleTypeDef>(htim10);

    Hw::adc = std::make_unique<ADC_HandleTypeDef>(adc);

    //init
    HAL_TIM_Encoder_Start(Hw::_encoder1.get(), TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(Hw::_encoder2.get(), TIM_CHANNEL_ALL);

    HAL_TIM_PWM_Start(Hw::pwm.get(), TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(Hw::pwm.get(), TIM_CHANNEL_2);

    HAL_TIM_Base_Start(Hw::delayTimer.get());

    //adc
    HAL_ADC_Start(Hw::adc.get());

    HAL_ADCEx_InjectedStart(Hw::adc.get());

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

std::shared_ptr<Encoder> Hw::getEncoder2() {
    return Hw::encoder2;
}


bool Hw::isReady() {
    return Hw::ready;
}

// voltage calibration data
// 5v-142000
// 3.33-94000
void Hw::setVoltage(float voltage) {
    //convert voltage (presumably 0-12) into DAC voltage (0-3.3)
    float dacVoltage = voltage;

    //use DAC calibration values to acquire bit values for dac upper bits
    uint8_t dacBits = 0;
    //use all bits except 0, 0 bit will be controlled by pwm

    for (int i = voltageDACCall.size() - 1; i > 0; i--) {
        if (dacVoltage - voltageDACCall[i] >= 0) {
            dacBits |= 1 << i;
            dacVoltage -= voltageDACCall[i];
        }
    }
    if (dacVoltage > voltageDACCall[0]) {
        //cant output higher voltage than required!
    }

    //convert leftover voltage to pwm ccr value
    uint32_t prc = int32_t(dacVoltage / voltageDACCall[0]) * Hw::pwm->Instance->ARR;//ARR-period, i.e. max value

    if (voltage < 0) {
        voltage = 0;
    }
    _setVoltage = voltage;
    //way to decrease measured voltage lag
    //Hw::voltageAverage.addPoint(_setVoltage);

//#define VOLTAGE_CONV(V) (28742.51497005988*(V)-1712.5748502994102)

    //set dacBits to output
    if (dacBits & (1 << 1)) {
        pinWrite(VDAC1, HIGH);
    } else {
        pinWrite(VDAC1, LOW);
    }
    if (dacBits & (1 << 2)) {
        pinWrite(VDAC2, HIGH);
    } else {
        pinWrite(VDAC2, LOW);
    }
    if (dacBits & (1 << 3)) {
        pinWrite(VDAC3, HIGH);
    } else {
        pinWrite(VDAC3, LOW);
    }
    if (dacBits & (1 << 4)) {
        pinWrite(VDAC4, HIGH);
    } else {
        pinWrite(VDAC4, LOW);
    }

    // set pwm to output
    Hw::pwm->Instance->CCR1 = prc;
    //Hw::pwm->Instance->CCR2 = 0;//current
}

void Hw::setCurrent(float current) {
    if (current < 0) {
        current = 0;
    }
    _setCurrent = current;
//#define CURRENT_CONV(I)  (28742.51497005988*(I)-1712.5748502994102)

    uint32_t cur = 463000 * _setCurrent - 4300;
    Hw::pwm->Instance->CCR2 = cur;
}


float Hw::getRealVoltage() {

    return Hw::voltageAverage.getAverage();
}

float Hw::getRealCurrent() {

    return Hw::currentAverage.getAverage();
}

float Hw::getSetVoltage() {
    return _setVoltage;
}

float Hw::getSetCurrent() {
    return _setCurrent;
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
        Hw::encoder1->value = (_encoder1->Instance->CNT >> 2);//Different kind of encoder!!!
    } else {
        Hw::encoder1->hasChanged = false;
    }

    uint32_t enc2Val = _encoder2->Instance->CNT >> 2;
    if (Hw::encoder2->value != enc2Val) {

        int32_t rawdiff = Hw::encoder2->value - enc2Val;

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

        Hw::encoder2->hasChanged = true;
        Hw::encoder2->diff = rawdiff;
        Hw::encoder2->value = (_encoder2->Instance->CNT >> 2);
    } else {
        Hw::encoder2->hasChanged = false;
    }


}

State Hw::pinRead(Pin pin) {
    return static_cast<State>(HAL_GPIO_ReadPin(pin.GPIO, pin.GPIO_Pin));
}

void Hw::adcSampler() {
    while (true) {
        //uint16_t vref = 0;
        //uint16_t volt = 0;
        auto a = *VREFINT_CAL_ADDR;
        (void) a;
        float avg = 0;
        for (int i = 0; i < 10; ++i) {
            uint16_t vref = HAL_ADCEx_InjectedGetValue(Hw::adc.get(), ADC_INJECTED_RANK_1);
            uint16_t data = HAL_ADCEx_InjectedGetValue(Hw::adc.get(), ADC_INJECTED_RANK_2);
            //supply voltage
            float vdd = 3.3 * float(*VREFINT_CAL_ADDR) / vref;
            avg += (vdd / 4095) * float(data);
            osDelay(10);
        }
        avg /= 10;


        Hw::voltageAverage.addPoint(avg * 8.068687 - 0.567394);

#define VOLTAGE_DIVIDER(V) ((V)*((130000.0+20000.0)/20000.0))
        //uint16_t vref = 0;
        //uint16_t volt = 0;
        uint16_t vref = HAL_ADCEx_InjectedGetValue(Hw::adc.get(), ADC_INJECTED_RANK_1);
        uint16_t adc = HAL_ADCEx_InjectedGetValue(Hw::adc.get(), ADC_INJECTED_RANK_3);

        float vdd = 1.20 * 4096.0 / vref;
        float actualCurrent = VOLTAGE_DIVIDER(adc * vdd / 4096.0);
        Hw::currentAverage.addPoint(actualCurrent);

        //osDelay(100);
    }
}

