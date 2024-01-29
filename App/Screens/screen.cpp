//
// Created by vm on 24.16.1.
//

#include <cstring>
#include "screen.h"
#include "Renderer/renderer.h"
#include "Utils/strUtils.h"

void Screen::update() {

}

void Screen::print(Place place, const std::string &str) {
    std::memcpy(&place, str.c_str(), str.size());
}

void Screen::print(Place place, char ch) {
    place = ch;

}

void Screen::setNewScreen(ScreenType type) {
    auto msg = new RendererMessage();
    msg->screenID0 = type;
    osStatus_t status = osMessageQueuePut(this->queue, msg, 0, 0);
    (void) status;
}

void Screen::setup(osMessageQueueId_t qu) {
    this->queue = qu;
    this->_setup();

}

void Screen::printLargeFloat(Place place, const std::string &str, uint8_t selectedDigit, bool blink) {
    uint8_t numpos = 0;
    uint8_t colpos = 0;
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
//            if (!(numpos == selectedDigit && blink)) {
            LargeFont::printChar(place[0][colpos], str[i]);
            colpos += 2;
            numpos += 1;
        } else if (str[i] == '.') {
            place[1][colpos] = 111;//large dot
            colpos += 1;
        }
    }
}

void Screen::printf(Place place, float num, uint8_t selectedDigit, bool blink) {
    std::string s = ftos(num);
    uint8_t digi = 0;
    for (int i = 0; i < 5; i++) {
        if (!(digi == selectedDigit && blink)) {
            place[0][i] = s[i];
        }
        if(isdigit(s[i])){
            digi+=1;
        }
    }
}


