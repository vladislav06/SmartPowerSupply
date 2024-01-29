//
// Created by vm on 24.16.1.
//
#pragma once

#include "Drivers/largeFont.h"
#include "Hw/Hw.h"
#include "cmsis_os.h"
#include "screenType.h"
#include "buffer.h"


class Screen {
protected:
    explicit Screen(std::string name) : name(std::move(name)) {};

    //helper methods

    static void print(Place place, const std::string &str);

    static void print(Place place, char ch);

    /// Will print large float at specified place in buffer, and will blink with specified digit
    void printLargeFloat(Place place, const std::string &num, uint8_t selectedDigit, bool blink);

    /// Will print float with 2 leading zeros, and with precision of 2 digits after coma
    void printf(Place place, float num, uint8_t selectedDigit, bool blink);

    void setNewScreen(ScreenType type);

    virtual void _setup() = 0;

    osMessageQueueId_t queue = nullptr;
public:
    const std::string name;

    virtual ScreenType getType() = 0;


    /// Method that renders screen
    /// @param screen - empty screen that this method fills up, screen is 4x20
    /// @param largeFont - large font helper
    /// @param blink - whether cursor is shown or hidden
    virtual void render(Buffer &screen, bool blink) = 0;

    /// This Method will be called when screen is shown to the user,
    /// Use for uploading custom font
    /// @param qu queue for updating currently drawn screen
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

