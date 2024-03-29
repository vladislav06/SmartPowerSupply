//
// Created by vm on 24.17.1.
//
#pragma once

#include <memory>
#include <array>
#include <vector>
#include "cmsis_os.h"
#include "Screens/screen.h"
#include "Screens/buffer.h"
#include "rendererConfig.h"


typedef struct {                                // object data type
    std::shared_ptr<Screen> screen;
} RendererMessage;

class Renderer {
private:
    /// represents string of chars that are different
    struct Block {
        //starting row_
        uint8_t row;
        //starting column
        uint8_t col;
        //string
        std::vector<char> chars;
    };

    /// Current screen
    std::shared_ptr<Screen> screen;

    Buffer buffer;
    Buffer nextBuffer;

    osMessageQueueId_t queue;


    void showOnDisplay(const std::vector<Block> &diff);

    /// Queue for notification


public:
    Renderer();

    void init(osMessageQueueId_t rq);

    /// Will start main rendering loop
    [[noreturn]] void start();

    void setScreen(std::shared_ptr<Screen> scr);

    // fast drawing method that updates only those symbols that have been changed
    void draw();

    // Clears buffer, so next draw will be full redraw
    // sometimes errors occur, and display needs full refresh
    void fullReDraw();

    std::shared_ptr<Screen> getCurrentScreen();


};
