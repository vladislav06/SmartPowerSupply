//
// Created by vm on 24.19.1.
//
#pragma once

#include "screen.h"
#include "screens.h"


class MenuScreen : public Screen, ScreenRegisterer<MenuScreen, ScreenType::MENU> {
private:
    // index of currently selected screen
    int selected = 0;
    int encoderStartValue = 0;
    std::shared_ptr<Screen> selectedScreen{};
    //custom "->" symbol

    uint8_t pointer[8] = {
            0b00000,
            0b00100,
            0b00010,
            0b11111,
            0b00010,
            0b00100,
            0b00000,
            0b00000
    };
public:
    MenuScreen() : Screen("Screen") {};

    void render(std::array<std::string, 4> &screen) override;

    void onEncoder1Update() override;

    void onEncoder2Update() override;

    void onButtonPress(std::shared_ptr<Button> button) override;

    void _setup() override;

    ScreenType getType() override;

};
