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
    osTimerId_t blinkTimer;
    Blinker blinker{2};
    uint8_t  selectedDigit = 0;
public:
    PowerSupplyScreen() : Screen("PowerSupply") {
        blinkTimer = osTimerNew(
                [](void *s) {
                    auto *self = (PowerSupplyScreen *) s;
                    //enable blinking
                    self->blinker.blink(true);
                },
                osTimerOnce,
                this,
                NULL);
    };

    void render(Buffer &screen) override;

    void onEncoder1Update(int difference) override;

    void onEncoder2Update() override;

    void onButtonPress(std::shared_ptr<Button> button) override;

    ScreenType getType() override;

protected:
    void _stop() override;

    void _start() override;

};

