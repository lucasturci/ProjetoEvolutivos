/*
Desenvolvedores:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <cstdlib>
#include "constants.h"
#include "RandomNumber.hpp"


class Circle {
public:
    static const int seed = 205;
    const float vel = 5;
    float radius = 20;
    float x, y;
    bool horizontal;
    int orientation; // 0 = positive 1 = negative
    bool active;
    int iterations_until_active;
    static RandomNumber * gen;
    Circle() {
        // init its position

        if(gen == NULL) gen = RandomNumber::getGenerator();
        horizontal = gen->randInt(0, 1);
        orientation = gen->randInt(0, 1);
        x = gen->randInt(radius, window_width - radius);
        y = gen->randInt(radius, window_height - radius);

        active = false;
        iterations_until_active = 100;

    }

    static void setGenerator() {
        gen = new RandomNumber(seed);
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void move() {
        // horizontal circle
        if(horizontal) {
            if(orientation == 0) {
                x += vel;
                if(x + radius >= window_width) {
                    x -= (x + radius - (window_width - 1)); // kickback
                    orientation = 1 - orientation;
                }
            } else {
                x -= vel;
                if(x - radius < 0) {
                    x += 0 - (x - radius); // kickback
                    orientation = 1 - orientation;
                }
            }
        } else { // vertical circle
            if(orientation == 0) {
                y += vel;
                if(y + radius >= window_height) {
                    y -= (y + radius - (window_height - 1)); // kickback
                    orientation = 1 - orientation;
                }
            }  else {
                y -= vel;
                if(y - radius < 0) { // kickback
                    y += 0 - (y - radius);
                    orientation = 1 - orientation;
                }
            }
        }

        iterations_until_active--;
        if(iterations_until_active == 0) {
            active = true;
        }
    }

    ~Circle() {
        // does nothing
    }
};

RandomNumber * Circle::gen = NULL;

#endif