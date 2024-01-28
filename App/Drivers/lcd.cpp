//
// Created by vm on 24.14.1.
//

#include "lcd.h"
#include "cmsis_os.h"
#include "Hw/Hw.h"

Lcd::Lcd(I2C_HandleTypeDef &i2c, uint8_t i2caddr) : i2c(i2c), i2caddr(i2caddr) {
}


void Lcd::init() {
    //register i2c callback
    //HAL_I2C_RegisterCallback(&i2c,);

    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;

    _numlines = 4;


    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    osDelay(50);

    // Now we pull both RS and R/W low to begin commands
    expanderWrite(_backlightval);    // reset expander and turn backlight off (Bit 8 =1)
    osDelay(1000);



    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    osDelay(5); // wait min 4.1ms

    // second try
    write4bits(0x03 << 4);
    osDelay(5); // wait min 4.1ms

    // third go!
    write4bits(0x03 << 4);
    osDelay(1);

    // finally, set to 4-bit interface
    write4bits(0x02 << 4);


    // set # lines, loadFont size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for roman languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);

    home();
}

/********** high level commands, for the user! */
void Lcd::clear() {
    command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
    osDelay(2);  // this command takes a long time!
}

void Lcd::home() {
    command(LCD_RETURNHOME);  // set cursor position to zero
    osDelay(2);  // this command takes a long time!
}

void Lcd::setCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > _numlines) {
        row = _numlines - 1;    // we count rows starting w/0
    }
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void Lcd::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void Lcd::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void Lcd::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void Lcd::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void Lcd::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void Lcd::blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void Lcd::scrollDisplayLeft() {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void Lcd::scrollDisplayRight() {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void Lcd::leftToRight() {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void Lcd::rightToLeft() {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void Lcd::autoscroll() {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void Lcd::noAutoscroll() {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Lcd::createChar(uint8_t location, const uint8_t charmap[]) {
    //location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

//createChar with PROGMEM input
void Lcd::createChar(uint8_t location, char *charmap) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(*(charmap++));
    }
}

// Turn the (optional) backlight off/on
void Lcd::noBacklight(void) {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void Lcd::backlight(void) {
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(0);
}


/*********** mid level commands, for sending data/cmds */

void Lcd::command(uint8_t value) {
    send(value, 0);
}

void Lcd::write(uint8_t value) {
    send(value, Rs);
}

/************ low level data pushing commands **********/

// write either command or data
void Lcd::send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

void Lcd::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void Lcd::expanderWrite(uint8_t data) {
    data |= _backlightval;
    HAL_I2C_Master_Transmit(&i2c, i2caddr << 1, &data, 1, 100);
}

void Lcd::pulseEnable(uint8_t _data) {
    expanderWrite(_data | En);    // En high
    Hw::delauUs(1);        // enable pulse must be >450ns

    expanderWrite(_data & ~En);    // En low
    Hw::delauUs(50);        // commands need > 37us to settle
}

void Lcd::print(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        write(str[i]);
    }
}
