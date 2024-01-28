//
// Created by vm on 24.16.1.
//

#include <cstring>
#include "screen.h"
#include "screens.h"
#include "Renderer/renderer.h"

void Screen::update() {

}

void Screen::print(char *place, const std::string &str) {
    std::memcpy(place, str.c_str(), str.size());
}

void Screen::print(char *place, char ch) {
    *place = ch;

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


