//
// Created by vm on 24.16.1.
//
#pragma once

#include "screen.h"
#include "screens.h"
#include "buffer.h"

class PowerSupplyScreen : public Screen, ScreenRegisterer<PowerSupplyScreen, ScreenType::DEFAULT> {
public:
    PowerSupplyScreen() : Screen("PowerSupply") {};

    void render(Buffer &screen, bool blink) override;

    void onEncoder1Update() override;

    void onEncoder2Update() override;

    void onButtonPress(std::shared_ptr<Button> button) override;

    void _setup() override;

    ScreenType getType() override;


};

