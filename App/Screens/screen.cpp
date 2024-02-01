//
// Created by vm on 24.16.1.
//

#include <cstring>
#include "screen.h"
#include "Renderer/renderer.h"
#include "Utils/strUtils.h"
#include "usbd_cdc_if.h"
#include "Utils/debug.h"

void Screen::update() {

}

void Screen::print(Place place, const std::string &str) {
    //std::memcpy(&place, str.c_str(), str.size());
    for (int i = 0; i < str.size(); i++) {
        place[0][i]=str[i];
    }

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

void Screen::start(osMessageQueueId_t qu, Renderer *rndrConf) {
    this->renderer = rndrConf;
    this->queue = qu;
    this->_start();

}

void Screen::printLargeF(Place place, const std::string &num) {
    uint8_t numpos = 0;
    uint8_t colpos = 0;
    for (int i = 0; i < num.length(); i++) {
        if (isdigit(num[i])) {
//            if (!(numpos == selectedDigit && blink)) {
            LargeFont::printChar(place[0][colpos], num[i]);
            colpos += 3;
            numpos += 1;
        } else if (num[i] == '.') {
            place[1][colpos - 1] = 111;//large dot
            //colpos += 1;
        }
    }
}

void Screen::printf(Place place, float num, uint8_t selectedDigit, bool blink) {
    std::string s = ftos(num);
    for (int i = 0; i < 5; i++) {

        Debug::printf("i:%d; C: %c:   ", i, s[i]);

        if ((!(selectedDigit == i && blink)) || !isdigit(s[i])) {
            Debug::printf("printed!");
            place[0][i] = s[i];
        }
        if (!isdigit(s[i])) {
            selectedDigit += 1;
        }
        Debug::printf("\n");
    }
    Debug::printf("=====\n");
}

void Screen::stop() {
    _stop();
}


