//
// Created by vm on 24.19.1.
//

#include "menuScreen.h"
#include "array"

void MenuScreen::render(Buffer &screen) {
    // list all available screens, excluding menu
    for (int n = 0; n < screens.size() && n < 4; n++) {
        if (n == selected) {
            print(screen[n][0], 0);
            selectedScreen = screens[n];
        }
        print(screen[n][1], screens[n]->name);
    }
}

void MenuScreen::onEncoder1Update(int difference) {
    selected = (Hw::encoder1->value - encoderStartValue) % screens.size();
}

void MenuScreen::onEncoder2Update() {

}

void MenuScreen::onButtonPress(std::shared_ptr<Button> button) {
    switch (button->type) {
        case Button::MENU:
            setScreen(selectedScreen->getType());
            break;
        case Button::ENC1:
            break;
        case Button::ENC2:
            break;
    }

}

void MenuScreen::_start() {

    //save encoder value, for offset
    encoderStartValue = Hw::encoder1->value;

    //load custom symbol
    Hw::lcd->createChar(0, pointer);


}

ScreenType MenuScreen::getType() {
    return MENU;
}

void MenuScreen::_stop() {

}
