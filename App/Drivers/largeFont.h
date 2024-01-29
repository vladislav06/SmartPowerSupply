#pragma once
// Constants
#include "lcd.h"
#include "Screens/buffer.h"

#define BIGNUMCHARS     13    // The total number of characters in each loadFont


class LargeFont {
public:


    ///Loads font in to display
    static void loadFont(Lcd &lcd);

    static void printNum(Place dest, const uint8_t *Number);

    static void printChar(Place dest,const char &num);

    ///Will print string of numbers at specified starting position in an array of 4 strings
    static void prints(Place dest, std::string str);

};

