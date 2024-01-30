//
// Created by vm on 24.17.1.
//

#include "renderer.h"
#include "Hw/Hw.h"

#include <utility>


Renderer::Renderer() {
}

void Renderer::setScreen(std::shared_ptr<Screen> scr) {
    this->screen = std::move(scr);
    this->screen->setup(queue, this);
}


void Renderer::showOnDisplay(const std::vector<Block> &diff) {
    for (auto block: diff) {
        Hw::lcd->setCursor(block.col, block.row);
        for (int i = 0; i < block.chars.size(); i++) {
            Hw::lcd->write(block.chars[i]);
        }
    }
}

void Renderer::draw() {

    //draw screen
    screen->render(nextBuffer, blinkState);

    // compare buffer and nextbuffer,
    // store difference as vector of vectors that contain chars(to remove string concatenation) that are different
    std::vector<Block> diff;

    //stores position of last different char
    uint8_t lastCol = 0XFF;//some value out of possible display index

    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 20; col++) {
            if (buffer[row][col] != nextBuffer[row][col]) {
                if (col == lastCol + 1) {
                    //append to prev vector
                    diff.back().chars.push_back(nextBuffer[row][col].c());
                } else {
                    //create block
                    Block blck;
                    blck.col = col;
                    blck.row = row;
                    blck.chars.push_back(nextBuffer[row][col].c());
                    diff.push_back(blck);
                }
                lastCol = col;
            }
        }
    }
    //
    showOnDisplay(diff);
    buffer = nextBuffer;
    nextBuffer.clear();

}


void Renderer::fullReDraw() {
    buffer.clear();

}

std::shared_ptr<Screen> Renderer::getCurrentScreen() {
    return screen;
}

void callback(void *ths) {
    auto self = (Renderer *) ths;
    self->blink();
}

void Renderer::init(osMessageQueueId_t rq) {
    this->queue = rq;

    //create thread for blink
    const osThreadAttr_t blink_attributes = {
            .name = "blink",
            .stack_size = 256 * 2,
            .priority = (osPriority_t) osPriorityNormal,
    };
    ///pass this to allow method call
    blinkHandle = osThreadNew(callback, this, &blink_attributes);
}

void Renderer::blink() {
    //blink
    while (true) {
        if (this->doBlink) {
            this->blinkState = !blinkState;
        }
        osDelay(500);
    }
}

bool Renderer::getBlinkState() {
    return blinkState;
}

void Renderer::blink(bool doBlink) {
    this->doBlink = doBlink;

}

void Renderer::setBlinkSate(bool blink) {
    this->blinkState = blink;

}



