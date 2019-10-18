#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "constants.h"

class Circle {
public:
    const float vel = 5;
    float radius = 20;
    float x, y;
    bool horizontal;
    int orientation; // 0 = positive 1 = negative
    bool active;
    int iterations_until_active;

    Circle() {
        // init its position
        horizontal = rand()%2;
        orientation = rand()%2;
        x = radius + rand()%int(window_width - 2 * radius);
        y = radius + rand()%int(window_height - 2 * radius);
        active = false;
        iterations_until_active = 100;

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

#endif