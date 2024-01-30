//
// Created by vm on 24.16.1.
//
#pragma once

#include "screen.h"
#include "screens.h"
#include "buffer.h"
#include "Renderer/renderer.h"

class PowerSupplyScreen : public Screen, ScreenRegisterer<PowerSupplyScreen, ScreenType::DEFAULT> {
private:
    osTimerId_t blinkTimer;
public:
    PowerSupplyScreen() : Screen("PowerSupply") {
        blinkTimer = osTimerNew([](void *s) {
            auto *self = (PowerSupplyScreen *) s;
            //enable blinking
            self->renderer->blink(true);


        }, osTimerOnce, this, NULL);
    };

    void render(Buffer &screen, bool blink) override;

    void onEncoder1Update() override;

    void onEncoder2Update() override;

    void onButtonPress(std::shared_ptr<Button> button) override;

    void _setup() override;

    ScreenType getType() override;


};

