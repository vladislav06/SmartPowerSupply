//
// Created by vm on 24.16.1.
//

#include <cstring>
#include "powerSupplyScreen.h"
#include "array"
#include "Utils/strUtils.h"
#include "App/Hw/Hw.h"
#include "buffer.h"
#include "Renderer/renderer.h"

void PowerSupplyScreen::render(Buffer &screen, bool blink) {
    //screen[1][0] = 'V';
    printLargeFloat(screen[0][0], ftos(Hw::getEncoder1()->value), 1, blink);

    // screen[3][0] = 'A';
    printLargeFloat(screen[2][0], ftos(Hw::getRealCurrent()), 1, blink);


    print(screen[0][11], "set:");
    screen[1][13] = 'V';
    screen[1][14] = ':';
    //print(screen[1][15], ftos(Hw::getRealVoltage()));
    printf(screen[1][15], Hw::getRealVoltage(), 2, blink);

    // print(screen[2][11], "max:");
    screen[2][13] = 'A';
    screen[2][14] = ':';
    print(screen[2][15], ftos(Hw::getEncoder1()->value));
    print(screen[3][11], "Warn:");
    print(screen[3][16], "CCCV");
}

void PowerSupplyScreen::onEncoder1Update() {
    //this->update();
    this->renderer->blink(false);
    this->renderer->setBlinkSate(false);
  //  osTimerStop(blinkTimer);
    osTimerStart(blinkTimer, 1000);



}

void PowerSupplyScreen::onEncoder2Update() {
    //  this->update();
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
