//
// Created by vm on 24.29.1.
//
#include "buffer.h"

Buffer::Row Buffer::operator[](int row) {
    return Buffer::Row(this, row, 0);
}

Place Buffer::Row::operator[](int col) {
    return Place(buffer, row_, col_ + col);
}
