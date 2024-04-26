//
// Created by vm on 24.16.1.
//
#pragma once

#include "screen.h"
#include "screens.h"
#include "buffer.h"
#include "Renderer/renderer.h"
#include "blinker.h"

class PowerSupplyScreen : public Screen, ScreenRegisterer<PowerSupplyScreen, ScreenType::DEFAULT> {
private:
    osTimerId_t voltageBlinkTimer;
    Blinker voltageBlinker{2};

    osTimerId_t currentBlinkTimer;
    Blinker currentBlinker{2};

    uint8_t voltageSelectedDigit = 0;
    uint8_t currentSelectedDigit = 0;
public:
    PowerSupplyScreen() : Screen("PowerSupply") {
        voltageBlinkTimer = osTimerNew(
                [](void *s) {
                    auto *self = (PowerSupplyScreen *) s;
                    //enable blinking
                    self->voltageBlinker.blink(true);
                },
                osTimerOnce,
                this,
                NULL);

        currentBlinkTimer = osTimerNew(
                [](void *s) {
                    auto *self = (PowerSupplyScreen *) s;
                    //enable blinking
                    self->currentBlinker.blink(true);
                },
                osTimerOnce,
                this,
                NULL);
    };

    void render(Buffer &screen) override;

    void onEncoder1Update(int difference) override;

    void onEncoder2Update(int difference) override;

    void onButtonPress(std::shared_ptr<Button> button) override;

    ScreenType getType() override;

protected:
    void _stop() override;

    void _start() override;

};

