//
// Created by vm on 24.19.1.
//
#pragma once

#include "screen.h"
#include "screenType.h"


inline std::vector<std::shared_ptr<Screen>> screens{};

/// Extending this class will add new Screen instance to a global vector of screen instances.\n
/// Only works for classes that extend Screen class
template<typename T, ScreenType N>
class ScreenRegisterer {

public:
    static ScreenType getType() {
        return N;
    }

    ScreenRegisterer() {
        static_assert(std::is_base_of<Screen, T>::value, "T must derive from Screen");
        (void *) registered;
    }

private:
    static bool reg() {
        if (N >= screens.size()) {
            screens.resize(screens.size() + (N - screens.size() + 1), nullptr);

            screens[N] = static_cast<const std::shared_ptr<Screen>>(std::make_unique<T>());
        } else {
            screens[(int) N] = std::make_unique<T>();
        }
        int a = (int) N;
        (void *) a;
        return true;
    }

    static inline bool registered = reg();

};

