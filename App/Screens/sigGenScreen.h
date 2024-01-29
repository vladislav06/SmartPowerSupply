//
// Created by vm on 24.28.1.
//
#pragma once

#include "screen.h"
#include "screens.h"
#include "buffer.h"

class SigGenScreen : public Screen, ScreenRegisterer<SigGenScreen, ScreenType::SIG_GEN> {
protected:
    void _setup() override;

public:
    SigGenScreen() : Screen("SignalGen") {};

    ScreenType getType() override;

    void render(Buffer &screen, bool blink) override;

    void onEncoder1Update() override;

    void onEncoder2Update() override;

    void onButtonPress(std::shared_ptr<Button> button) override;

};

