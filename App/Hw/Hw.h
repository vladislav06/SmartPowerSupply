//
// Created by vm on 24.14.1.
//
#pragma once

#include "main.h"
#include "App/Drivers/lcd.h"
#include <memory>
#include <vector>

#define INIT(type) std::unique_ptr<type>(nullptr)

enum State {
    LOW = 0,
    HIGH = 1,
};

struct Pin {
    GPIO_TypeDef *GPIO;
    uint16_t GPIO_Pin;
};

struct Button {
    enum Type {
        MENU,
        ENC1,
        ENC2,

    };
    Pin pin{};

    State state{};

    ///Button type
    Type type{};

    /// Is true if state has been changed
    bool hasChanged = false;

    /// Is true if button went from unpressed state to pressed state
    bool risingEdge = false;

    /// Is true if button went from pressed state to unpressed state
    bool fallingEdge = false;

    Button(Pin pin, Type type) {
        this->pin = pin;
        this->type = type;
    }

    void reset() {
        hasChanged = false;
        risingEdge = false;
        fallingEdge = false;
    }
};

struct Encoder {
    /// Value cant be changed
    uint32_t value = 0;
    //// Difference form previous update
    int diff = 0;
    bool hasChanged = false;
};

class Hw {
private:
    inline static bool ready = false;
    inline static Hw *instance = nullptr;
public:
    Hw() {
        instance = this;
    }


    // hardware
    inline static std::unique_ptr<Lcd> lcd{};
    inline static std::unique_ptr<ADC_HandleTypeDef> adc{};

    // interface
    inline static std::unique_ptr<I2C_HandleTypeDef> i2c{};

    //timers
    inline static std::unique_ptr<TIM_HandleTypeDef> htim1{};
    inline static std::unique_ptr<TIM_HandleTypeDef> htim3{};
    inline static std::unique_ptr<TIM_HandleTypeDef> _encoder1{};
    inline static std::unique_ptr<TIM_HandleTypeDef> delayTimer{};

    //encoders
    inline static std::shared_ptr<Encoder> encoder1{new Encoder};
    inline static std::shared_ptr<Encoder> encoder2{new Encoder};

    // Used Pins
    inline const static Pin PC13 = {GPIOC, GPIO_PIN_13};
    inline const static Pin PB5 = {GPIOB, GPIO_PIN_5};
    inline const static Pin PB4 = {GPIOB, GPIO_PIN_4};
    inline const static Pin PB3 = {GPIOB, GPIO_PIN_3};

    inline static std::shared_ptr<Button> menuButton{new Button(PB5, Button::MENU)};
    inline static std::vector<std::shared_ptr<Button>> buttons{
            menuButton
    };

    /// Will start hardware
    static void start(ADC_HandleTypeDef &adc,
                      I2C_HandleTypeDef &i2c,
                      TIM_HandleTypeDef &htim2,
                      TIM_HandleTypeDef &htim3,
                      TIM_HandleTypeDef &htim4,
                      TIM_HandleTypeDef &htim10);

    static void init();

    static bool isReady();

    /// Sample all buttons
    static void sampleHardware();


    static void pinWrite(Pin pin, State state);

    static State pinRead(Pin pin);

    static void delayUs(uint16_t us);

    static std::shared_ptr<Encoder> getEncoder1();


    //output voltage control

    inline static float _setVoltage = 0;
    inline static float _realVoltage = 0;

    static void setVoltage(float voltage);
    static float getSetVoltage();

    static float getRealVoltage();

    //output current control

    static float getRealCurrent();

    static float getSetCurrent();
};


