//
// Created by vm on 24.17.1.
//
#pragma once

#include <memory>
#include <array>
#include <vector>
#include "Screens/screen.h"
#include "cmsis_os.h"
#include "Screens/screens.h"


typedef struct {                                // object data type
    uint8_t screenID0;
} RendererMessage;

class Renderer {
private:
    /// represents string of chars that are different
    struct Block {
        //starting row
        uint8_t row;
        //starting column
        uint8_t col;
        //string
        std::vector<char> chars;
    };

    /// Current screen
    std::shared_ptr<Screen> screen;

    std::array<std::string, 4> buffer;
    std::array<std::string, 4> nextBuffer;

    void showOnDisplay(const std::vector<Block> &diff);

    /// Queue for notification
    osMessageQueueId_t queue;


public:
    Renderer();

    void init( osMessageQueueId_t rq);

    void setScreen(std::shared_ptr<Screen> scr);

    // fast drawing method that updates only those symbols that have been changed
    void draw();

    // Clears buffer, so next draw will be full redraw
    // sometimes errors occur, and display needs full refresh
    void fullReDraw();

    std::shared_ptr<Screen> getCurrentScreen();

};
