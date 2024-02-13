//
// Created by vm on 24.31.1.
//

#include "blinker.h"


Blinker::Blinker(float freq) : speed((int) ((1 / freq) * 1000)) {
    const osThreadAttr_t blink_attributes = {
            .name = "blink",
            .stack_size = 256 * 1,
            .priority = (osPriority_t) osPriorityLow,
    };

    blinkHandle = osThreadNew(
            [](void *ths) {
                auto self = (Blinker *) ths;
                self->doBlink();
            },
            this,
            &blink_attributes);
    osThreadSuspend(blinkHandle);
};


void Blinker::start() {
    osThreadResume(blinkHandle);
    blink(true);
}

void Blinker::stop() {
    osThreadSuspend(blinkHandle);
}

void Blinker::blink(bool doBlink) {
    blinking = doBlink;
}

bool Blinker::blink() {
    return this->blinkState;
}

void Blinker::setBlink(bool blnk) {
    this->blinkState = blnk;
}

void Blinker::doBlink() {
    //blink
    while (true) {
        if (this->blinking) {
            this->blinkState = !blinkState;
        }
        osDelay(speed);
    }
}




