//
// Created by vm on 24.1.2.
//
#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include "usbd_cdc_if.h"
#include "cmsis_os2.h"


class Debug {
public:
    template<typename ... Args>
    static void printf(std::string msg, Args ... args) {

        std::string m = string_format(msg, std::forward<Args>(args)...);
        CDC_Transmit_FS((uint8_t *) m.c_str(), m.length());
        osDelay(1);
    }
    static void print(std::wstring &str){
        CDC_Transmit_FS((uint8_t *) str.c_str(), str.length());
        osDelay(1);
    }

    template<typename ... Args>
    static std::string string_format(const std::string &format, Args ... args) {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
};

