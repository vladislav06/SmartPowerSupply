//
// Created by vm on 24.16.1.
//
#pragma once

#include "Drivers/largeFont.h"
#include "Hw/Hw.h"
#include "cmsis_os.h"
#include "screenType.h"


class Screen {
protected:
    explicit Screen(std::string name) : name(std::move(name)) {};

    static void print(char *place, const std::string &str);

    static void print(char *place, char ch);

    void setNewScreen(ScreenType type);

    virtual void _setup() = 0;

    osMessageQueueId_t queue = nullptr;
public:
    const std::string name;

    virtual ScreenType getType() = 0;


    /// Method that renders screen
    ///@param screen - empty screen that this method fills up, screen is 4x20
    /// @param largeFont - large font helper
    virtual void render(std::array<std::string, 4> &screen) = 0;

    /// This Method will be called when screen is shown to the user,
    /// Use for uploading custom font
    /// @param qu qu for updating currently drawn screen
    void setup(osMessageQueueId_t qu);

    /// This method will be called when encoder 1 is rotated
    virtual void onEncoder1Update() = 0;

    /// This method will be called when encoder 2 is rotated
    virtual void onEncoder2Update() = 0;

    /// This method will be called when any button is pressed
    /// @param button pointer to button that is pressed
    virtual void onButtonPress(std::shared_ptr<Button> button) = 0;

    /// Call this method when screen update is necessary
    void update();

    virtual ~Screen() = default;


};

