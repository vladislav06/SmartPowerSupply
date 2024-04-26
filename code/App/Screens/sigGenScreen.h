//
// Created by vm on 24.28.1.
//
#pragma once

#include "screen.h"
#include "screens.h"
#include "buffer.h"

class SigGenScreen : public Screen, ScreenRegisterer<SigGenScreen, ScreenType::SIG_GEN> {
protected:
    void _start() override;

public:
    SigGenScreen() : Screen("SignalGen") {};

    ScreenType getType() override;

    void render(Buffer &screen) override;

    void onEncoder1Update(int difference) override;

    void onEncoder2Update(int difference) override;

    void onButtonPress(std::shared_ptr<Button> button) override;
    void _stop() override;

};

