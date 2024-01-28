//
// Created by vm on 24.14.1.
//
#include <array>
#include <cmath>
#include <sstream>
#include "app.h"
#include "App/Hw/Hw.h"
#include "cmsis_os.h"
#include "Drivers/largeFont.h"
#include "Screens/screen.h"
#include "Screens/powerSupplyScreen.h"
#include "Renderer/renderer.h"
#include "Utils/rollingAverage.h"
#include "Screens/screens.h"
#include "Screens/menuScreen.h"


osMessageQueueId_t rq = osMessageQueueNew(10, sizeof(RendererMessage), NULL);;

[[noreturn]] void app() {
    //init hardware
    Hw();
    Hw::init();

    (void) screens.size();
    //draw loop

    for (;;) {
        Hw::sampleHardware();



        Hw::pinWrite(Hw::PC13, Hw::menuButton->state);
        Hw::voltage = Hw::menuButton->state;
        osDelay(100);

    }
}


[[noreturn]] void render() {
    //wait until hardware is ready
    while (!Hw::isReady()) {
        osDelay(10);
    }

    auto rollingAverage = RollingAverage(10);
    rollingAverage.addPoint(1000);

    Renderer renderer;
    renderer.init(rq);

    renderer.setScreen(screens[ScreenType::DEFAULT]);


    int32_t diff;
    Hw::_encoder1->Instance->CNT = 0;
    //rendering loop
    for (;;) {
        osDelay(30);

        //renderer.fullReDraw();


        //receive message
        RendererMessage msg;
        osStatus_t status = osMessageQueueGet(rq, &msg, NULL, 0);
        if (status == osOK) {
            renderer.setScreen(screens[msg.screenID0]);
        }

        // updates

        for (const auto &btn: Hw::buttons) {
            if (btn->risingEdge) {
                renderer.getCurrentScreen()->onButtonPress(btn);
                btn->reset();

            }
        }

        if (Hw::encoder1->hasChanged) {
            renderer.getCurrentScreen()->onEncoder1Update();
            Hw::encoder1->hasChanged = false;
        }


        renderer.draw();
    }
}

