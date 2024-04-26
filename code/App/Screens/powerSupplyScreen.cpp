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
    printf(screen[1][15], Hw::getSetVoltage(), 3 - voltageSelectedDigit, voltageBlinker.blink());
    //print(screen[1][15], ftos(Hw::getEncoder2()->value));
    // print(screen[2][11], "max:");
    screen[2][13] = 'A';
    screen[2][14] = ':';
    printf(screen[2][15], Hw::getSetCurrent(), 3 - currentSelectedDigit, currentBlinker.blink());
    //print(screen[2][15], ftos(Hw::getEncoder1()->value));
    print(screen[3][11], "Warn:");
    print(screen[3][16], "CCCV");
}

void PowerSupplyScreen::onEncoder1Update(int difference) {
    //disable blinking while value of encoder is changing
    voltageBlinker.blink(false);
    voltageBlinker.setBlink(false);
    osTimerStart(voltageBlinkTimer, 1000);

    // increase voltage based on selected digit
    Hw::setVoltage(Hw::getSetVoltage() + (0.001 * pow(10,  voltageSelectedDigit+ 1) * difference));
}

void PowerSupplyScreen::onEncoder2Update(int difference) {
    //disable blinking while value of encoder is changing
    currentBlinker.blink(false);
    currentBlinker.setBlink(false);
    osTimerStart(currentBlinkTimer, 1000);

    // increase current based on selected digit
    Hw::setCurrent(Hw::getSetCurrent() + (0.001 * pow(10, currentSelectedDigit + 1) * difference));
}

void PowerSupplyScreen::onButtonPress(std::shared_ptr<Button> button) {
    switch (button->type) {
        case Button::MENU:
            //setNewScreen(MENU);
            break;
        case Button::ENC1:
            voltageSelectedDigit = (voltageSelectedDigit + 1) % 4;
            break;
        case Button::ENC2:
            currentSelectedDigit = (currentSelectedDigit + 1) % 4;
            break;
    }
}

void PowerSupplyScreen::_start() {
    LargeFont::loadFont(*Hw::lcd);
    voltageBlinker.start();
    currentBlinker.start();
}

ScreenType PowerSupplyScreen::getType() {
    return DEFAULT;
}

void PowerSupplyScreen::_stop() {
    voltageBlinker.stop();
    currentBlinker.stop();
}
