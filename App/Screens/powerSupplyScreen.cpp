//
// Created by vm on 24.16.1.
//

#include <cstring>
#include "powerSupplyScreen.h"
#include "array"
#include "Utils/strUtils.h"
#include "App/Hw/Hw.h"

void PowerSupplyScreen::render(std::array<std::string, 4> &screen) {
    screen[1][0] = 'V';
    LargeFont::prints(screen, ftos(Hw::getEncoder1()->value), 1, 0);

    screen[3][0] = 'A';
    LargeFont::prints(screen, ftos(Hw::getRealCurrent()), 1, 2);

    std::strcpy(&screen[0][13], "set:");
    screen[0][19] = 8;
    screen[1][13] = 'V';
    screen[1][14] = ':';
    std::strcpy(&screen[1][15], ftos(Hw::getRealVoltage()).c_str());

    std::strcpy(&screen[2][13], "max:");
    screen[3][13] = 'A';
    screen[3][14] = ':';
    std::strcpy(&screen[3][15], ftos(Hw::getEncoder1()->value).c_str());
}

void PowerSupplyScreen::onEncoder1Update() {
    this->update();

}

void PowerSupplyScreen::onEncoder2Update() {
    this->update();
}

void PowerSupplyScreen::onButtonPress(std::shared_ptr<Button> button) {
    switch (button->type) {
        case Button::MENU:
            setNewScreen(MENU);
            break;
        case Button::ENC1:
            break;
        case Button::ENC2:
            break;
    }
}

void PowerSupplyScreen::_setup() {
    LargeFont::loadFont(*Hw::lcd);
}

ScreenType PowerSupplyScreen::getType() {
    return DEFAULT;
}
