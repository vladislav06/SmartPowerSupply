//
// Created by vm on 24.28.1.
//

#include "sigGenScreen.h"

void SigGenScreen::_start() {

}

ScreenType SigGenScreen::getType() {
    return SIG_GEN;
}

void SigGenScreen::render(Buffer &screen) {

}

void SigGenScreen::onEncoder1Update(int difference) {

}

void SigGenScreen::onEncoder2Update() {

}

void SigGenScreen::onButtonPress(std::shared_ptr<Button> button) {
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

void SigGenScreen::_stop() {

}
