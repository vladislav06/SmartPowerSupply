/************************************************

  LargeFont displays a big number that spans 2x2 characters on a character LCD.
  - This library is designed to work with common 2x16 character LCDs, though may work with a variety of sizes.
  - It is used in conjunction with the Arduino LiquidCrystal library.

  Four different fonts are included, they can be easily modified or deleted if unused to save memory.
  Tron and Trek seem to be the easiest to read at a glance from far away (bold style).

  Copyright 2018, by Carrie Sundra for Alpenglow Industries
  All rights reserved.
  License: MIT

  ************************************************/

#include <array>
#include "largeFont.h"


//////////////////////////////////////////////////////////////////
// Creates the user-defined LCD characters.
// The below format does take up a lot of screen space,
//  but you can actually visualize the characters.
//////////////////////////////////////////////////////////////////
// 8 custom characters for Trek loadFont
// These will be loaded into the LCD if this loadFont is selected

uint8_t trek0[8] = {
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
};

uint8_t trek1[8] = {
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
};

uint8_t trek2[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111,
};

uint8_t trek3[8] = {
        0b11111,
        0b11111,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b11111,
        0b11111,
};

uint8_t trek4[8] = {
        0b11111,
        0b11111,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11111,
        0b11111,
};

uint8_t trek5[8] = {
        0b11111,
        0b11111,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
};

uint8_t trek6[8] = {
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b11111,
        0b11111,
};

uint8_t trek7[8] = {
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11111,
        0b11111,
};



/////////////////////////////////////////////////////////////////////
// Creates a Big Number - an array of the 4 LCD characters used for each number.
// The order is: (upper left, lower left, upper right, lower right)
// In the array, 0-7 are the eight custom characters, 255 is all on (full black), 32 is all off (space/blank)
// Others are special LCD characters used for the % symbol (these are pre-defined in the LCD)
// Example: Tron0 is an array of the 4 custom characters needed to create the number 0.
//
// The MegaXXXX array are pointers to the first character in each Big Number.
// (Remember your C pointer shorthand: Tron0 = &Tron0[0] = the address of the first element of Tron0)
// The MegaXXXX array is organized so that the index of the array contains the info for that number.
// Ex: MegaTron[0] = Tron0 = the 4 characters needed to print a Big Number "0"
// The MegaXXXX array makes it easy to copy the entire font into the current loadFont variable, MegaFont.
//
// There are a dozen other ways to do this, with 2 dimensional arrays or more pointers.
// I chose the below method because:
//   - each number and loadFont are very clearly spelled out
//   - characters should be easy to change
//   - pointer use is kept to a minimum in accordance with Arduino guidelines
//   - it's hopefully understandable for beginners and an OK intro to pointer use
/////////////////////////////////////////////////////////////////////

uint8_t Trek0[4] = {5, 7, 255, 6};
uint8_t Trek1[4] = {0, 2, 1, 7};
uint8_t Trek2[4] = {0, 4, 3, 2};
uint8_t Trek3[4] = {0, 2, 3, 3};
uint8_t Trek4[4] = {1, 0, 1, 5};
uint8_t Trek5[4] = {4, 2, 0, 3};
uint8_t Trek6[4] = {5, 4, 0, 3};
uint8_t Trek7[4] = {0, 32, 3, 1};
uint8_t Trek8[4] = {4, 4, 3, 3};
uint8_t Trek9[4] = {4, 2, 3, 6};
uint8_t Trek10[4] = {32, 32, 32, 32};     // blank numeral
uint8_t Trek11[4] = {219, 47, 47, 219};   // percent symbol
uint8_t Trek12[4] = {2, 32, 2, 32};       // negative symbol
uint8_t *MegaTrek[BIGNUMCHARS] = {Trek0, Trek1, Trek2, Trek3, Trek4, Trek5, Trek6, Trek7, Trek8, Trek9, Trek10, Trek11,
                                  Trek12};


/////////////////////////////////////////////////////////////////////
// loads user-defined characters into LCD and the specified loadFont in MegaFont array
// call if you want to change fonts
/////////////////////////////////////////////////////////////////////

void LargeFont::loadFont(Lcd &lcd) {
    lcd.createChar(0, trek0);
    lcd.createChar(1, trek1);
    lcd.createChar(2, trek2);
    lcd.createChar(3, trek3);
    lcd.createChar(4, trek4);
    lcd.createChar(5, trek5);
    lcd.createChar(6, trek6);
    lcd.createChar(7, trek7);
}
/////////////////////////////////////////////////////////////////////
// prints one 2x2 Big Number to the LCD, at the "index" location
// - index location is the LCD index for the leftmost column of a 2x2 character
// - print order: top left, bottom left, top right, bottom right
/////////////////////////////////////////////////////////////////////

void LargeFont::printNum(std::array<std::string, 4> &dest, uint8_t col, uint8_t row, const uint8_t *Number) {
    uint8_t i, j, k = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            dest[row + j][col + i] = Number[k];
            ///lcd.setCursor(col + i, row + j);
            ///lcd.write(Number[k]);
            k++;
        }
    }
}

void LargeFont::prints(std::array<std::string, 4> &dest, std::string str, uint8_t col, uint8_t row) {
    uint8_t colpos = 0;
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            printNum(dest, col + colpos, row, MegaTrek[str[i] - '0']);
            colpos += 3;//each digit requires 3 places
        } else if (str[i] == '.') {
            dest[row + 1][col + colpos - 1] = 111;//large dot
            ///lcd.setCursor(col + colpos, row + 1);
            ///lcd.write(111);//large dot
            //colpos += 1;//dot only requires 1
        }
    }
}
