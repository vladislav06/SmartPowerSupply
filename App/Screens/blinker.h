//
// Created by vm on 24.31.1.
//


#include "cmsis_os2.h"

class Blinker {
    osThreadId_t blinkHandle{};
    bool blinkState = true;
    bool blinking = true;
    int speed=0;
    void doBlink();
public:
    Blinker(float freq);

    void start();

    void stop();

    void blink(bool doBlink);

    bool blink();

    void setBlink(bool blnk);
};


