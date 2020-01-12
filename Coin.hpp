/*
Developers:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

#ifndef COIN_HPP
#define COIN_HPP

#include "constants.h"
#include "RandomNumber.hpp"
#include <cstdlib>

class Coin {
public:
    static constexpr float radius = 10.0;
    float x, y;
    Coin() {
        RandomNumber * gen = RandomNumber::getGenerator();
        x = gen->randInt(radius, window_width - radius);
        y = gen->randInt(radius, window_height - radius);
    }

    Coin(float x, float y) {
        this->x = x;
        this->y = y;
    }

    ~Coin() {
        // does nothing
    }
};

#endif