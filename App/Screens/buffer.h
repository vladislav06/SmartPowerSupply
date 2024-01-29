//
// Created by vm on 24.29.1.
//
#pragma once

#include <array>
#include <cstdint>

struct Buffer;
struct Place;
#define MAX_ROW 4
#define MAX_COL 20
/// Buffer that contains 20x4 chars
struct Buffer {

    /// 20x4 buffer
    std::array<std::array<char, 20>, 4> buffer{};

    void clear() {
        for (int i = 0; i < 4; i++) {
            buffer[i].fill(' ');
        }
    }

    struct Row;

    Buffer::Row operator[](int row);
    explicit Buffer(){
       // volatile int *a=0x0;
       // *a=1;
    };
};


struct Buffer::Row {
    friend Buffer;
    friend Place;

    Buffer *buffer;
    int row_;
    int col_;


    Place operator[](int col);

private:
    explicit Row(Buffer *buff, int row, int col) : buffer(buff), row_(row), col_(col) {
        check();
    };
    void check() const{
        if(row_>=MAX_ROW||col_>=MAX_COL){
            throw std::exception();
        }
    }
};

/// Represents specific place in buffer
struct Place {
    friend Buffer;

    Buffer *buffer;

    int row_;
    int col_;


    Place &operator=(char c) {
        check();
        buffer->buffer[row_][col_] = c;
        return *this;
    }

    Buffer::Row operator[](int row) {
        check();
        return Buffer::Row(buffer, row_ + row, col_);

    }

    auto operator<=>(const Place &place) const {
        check();
        return this->buffer->buffer[row_][col_] <=> place.buffer->buffer[place.row_][place.col_];
    };

    auto operator==(const Place &place) const {
        check();
        return this->buffer->buffer[row_][col_] == place.buffer->buffer[place.row_][place.col_];
    };

    auto operator!=(const Place &place) const {
        check();
        return this->buffer->buffer[row_][col_] != place.buffer->buffer[place.row_][place.col_];
    };

    explicit operator char() const {
        check();
        return buffer->buffer[row_][col_];
    }

    char *operator&() const {
        check();
        return &buffer->buffer[row_][col_];
    }

    char c() {
        check();
        return buffer->buffer[row_][col_];
    }

private:
    explicit Place(Buffer *buff, int row, int col) : buffer(buff), row_(row), col_(col) {
        check();
    };

    void check() const{
        if(row_>=MAX_ROW||col_>=MAX_COL){
            throw std::exception();
        }
    }

};
