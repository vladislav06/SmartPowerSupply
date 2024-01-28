//
// Created by vm on 24.17.1.
//

#pragma once
#include <string>

/// Converts float to xx.xx format
std::string ftos(float f) {
    std::string str(6, ' ');
    std::sprintf(&str[0], "%05.2f", f);
    return str;
}

