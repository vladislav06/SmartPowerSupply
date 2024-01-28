#pragma once
// Constants
#include "lcd.h"

#define BIGNUMCHARS     13    // The total number of characters in each loadFont


class LargeFont {
public:


    ///Loads font in to display
    static void loadFont(Lcd &lcd);

    static void printNum(std::array<std::string, 4> &dest, uint8_t col, uint8_t row, const uint8_t *Number);

    ///Will print string of numbers at specified starting position in an array of 4 strings
    static void prints(std::array<std::string, 4> &dest, std::string str, uint8_t col, uint8_t row);
};

