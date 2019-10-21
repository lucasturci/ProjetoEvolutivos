#ifndef COIN_HPP
#define COIN_HPP

#include "constants.h"
#include "RandomNumber.hpp"
#include <cstdlib>

class Coin {
public:
    const float radius = 10.0;
    float x, y;

    Coin() {
        RandomNumber * gen = RandomNumber::getGenerator();
        x = gen->randInt(radius, window_width - radius);
        y = gen->randInt(radius, window_height - radius);
    }

    ~Coin() {
        // does nothing
    }
};

#endif