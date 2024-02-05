//
// Created by vm on 24.17.1.
//

#include "renderer.h"
#include "Hw/Hw.h"
#include "Utils/debug.h"

#include <utility>


Renderer::Renderer() {
}

void Renderer::setScreen(std::shared_ptr<Screen> scr) {
    //stop previous screen
    if (this->screen != nullptr) {
        this->screen->stop();
    }
    this->screen = std::move(scr);
    this->screen->start(queue, this);
}


void Renderer::showOnDisplay(const std::vector<Block> &diff) {
    //   Buffer debug;
    for (auto block: diff) {
        Hw::lcd->setCursor(block.col, block.row);
        // Place place = debug[block.row][block.col];
        for (int i = 0; i < block.chars.size(); i++) {
            Hw::lcd->write(block.chars[i]);
            //    place[0][i]=block.chars[i];
        }
    }
//    for (int i = 0; i < 4; i++) {
//        std::string str = "                    ";
//        for (int n = 0; n < 20; n++) {
//            char c = debug.buffer[i][n];
//            if (c > 16 && c != 255) {
//                str[n] = c;
//            }
//        }
//        Debug::printf(str);
//        Debug::printf("\n");
//    }
}

uint8_t prevCol = 0;
uint8_t prevRow = 0;

void Renderer::draw() {

    //draw screen
    screen->render(nextBuffer);
    //debug buffer
//    for (int i = 0; i < 4; i++) {
//        std::string str = "                    ";
//        for (int n = 0; n < 20; n++) {
//            char c = nextBuffer.buffer[i][n];
//            if (c > 16 && c != 255) {
//                str[n] = c;
//            }
//        }
//        Debug::printf(str);
//        Debug::printf("\n");
//    }

    // compare buffer and nextbuffer,
    // store difference as vector of vectors that contain chars(to remove string concatenation) that are different
    std::vector<Block> diff;

//    // slowly update whole screen char by char
//    Block blc;
//    blc.row = prevRow;
//    blc.col = prevCol;
//    blc.chars.push_back(nextBuffer[prevRow][prevCol].c());
//   // diff.push_back(blc);
//    prevCol++;
//
//    if (prevCol == 19) {
//        prevCol = 0;
//        prevRow++;
//    }
//    if (prevRow == 3) {
//        prevCol = 0;
//        prevRow = 0;
//    }

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
        lastCol = 0XFF;// reset last last different char column index
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


void Renderer::init(osMessageQueueId_t rq) {
    this->queue = rq;
}

[[noreturn]] void Renderer::start() {
    for (;;) {
        osDelay(30);

        //renderer.fullReDraw();


        //receive message
        RendererMessage msg;
        osStatus_t status = osMessageQueueGet(queue, &msg, NULL, 0);
        if (status == osOK) {
            setScreen(msg.screen);
        }

        // updates
        for (const auto &btn: Hw::buttons) {
            if (btn->risingEdge) {
                getCurrentScreen()->onButtonPress(btn);
                btn->reset();// will prevent triggering second time
            }
        }

        if (Hw::encoder1->hasChanged) {
            getCurrentScreen()->onEncoder1Update(Hw::encoder1->diff);
            Hw::encoder1->hasChanged = false;
            Hw::encoder1->diff = 0;
        }


        draw();
        //renderer.fullReDraw();
    }
}

