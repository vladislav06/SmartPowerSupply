//
// Created by vm on 24.16.1.
//

#include "powerSupplyScreen.h"
#include "Utils/strUtils.h"
#include "buffer.h"
#include "Renderer/renderer.h"
#include "math.h"
void PowerSupplyScreen::render(Buffer &screen) {
    //screen[1][0] = 'V';
    printLargeF(screen[0][0], ftos(Hw::getRealVoltage()));

    // screen[3][0] = 'A';
    printLargeF(screen[2][0], ftos(Hw::getRealCurrent()));


    print(screen[0][11], "set:");
    screen[1][13] = 'V';
    screen[1][14] = ':';
    //print(screen[1][15], ftos(Hw::getRealVoltage()));
    printf(screen[1][15], Hw::getSetVoltage(), 3-selectedDigit, blinker.blink());

    // print(screen[2][11], "max:");
    screen[2][13] = 'A';
    screen[2][14] = ':';
    print(screen[2][15], ftos(Hw::getEncoder1()->value));
    print(screen[3][11], "Warn:");
    print(screen[3][16], "CCCV");
}

void PowerSupplyScreen::onEncoder1Update(int difference) {
    //disable blinking while value of encoder is changing
    blinker.blink(false);
    blinker.setBlink(false);
    osTimerStart(blinkTimer, 1000);

    // increase voltage based on selected digit
    Hw::setVoltage(Hw::getSetVoltage()+(0.001*pow(10,selectedDigit+1) *difference));


}

void PowerSupplyScreen::onEncoder2Update() {
    //  this->update();
}

void PowerSupplyScreen::onButtonPress(std::shared_ptr<Button> button) {
    switch (button->type) {
        case Button::MENU:
            //setNewScreen(MENU);
            selectedDigit = (selectedDigit+1)%4;
            break;
        case Button::ENC1:
            break;
        case Button::ENC2:
            break;
    }
}

void PowerSupplyScreen::_start() {
    LargeFont::loadFont(*Hw::lcd);
    blinker.start();

}

ScreenType PowerSupplyScreen::getType() {
    return DEFAULT;
}

void PowerSupplyScreen::_stop() {
    blinker.stop();
}
