#ifndef COIN_HPP
#define COIN_HPP

#include "constants.h"
#include <cstdlib>

class Coin {
public:
    const float radius = 10.0;
    float x, y;

    Coin() {
        x = radius + rand()%int(window_width - 2 * radius);
        y = radius + rand()%int(window_height - 2 * radius);
    }

    ~Coin() {
        // does nothing
    }
};

#endif