#ifndef HERO_HPP
#define HERO_HPP

#include "constants.h"
#include "Coin.hpp"

class Hero {
public:
    const float radius = 20.0;
    const float accel = .5;
    const float deaccel = .2;
    float x, y;
    float vx, vy;
    bool collided;
    int score;
    int iterations;
    Coin * coin;

    Hero() {
        x = window_width/2;
        y = window_height/2; 
        vx = vy = 0;
        collided = false;
        iterations = 0;
        score = 0;
        makeCoin();
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void applyForce(int dx, int dy) {
        vx += dx * accel;
        vy += dy * accel;
    }

    bool gotCoin() {
        assert(coin);
        return sqrt((x - coin->x) * (x - coin->x) + (y - coin->y) * (y - coin->y) ) <= radius + coin->radius;
    }

    void move() {
        x += vx;
        if(x + radius >= window_width) { // doesnt kickback, but hits the wall
            x = window_width - radius;
            vx = 0;
        }
        if(x - radius < 0) { // doesnt kickback, but hits the wall
            x = radius;
            vx = 0;
        }

        y += vy;
        if(y + radius >= window_height) {// doesnt kickback, but hits the wall
            y = window_height - radius;
            vy = 0;
        }
        if(y - radius < 0) { // doesnt kickback, but hits the wall
            y = radius;
            vy = 0;
        }

        // deacceleration
        if(vx > 0) vx = std::max(0.0f, vx - deaccel);
        else vx = std::min(0.0f, vx + deaccel);
        if(vy > 0) vy = std::max(0.0f, vy - deaccel);
        else vy = std::min(0.0f, vy + deaccel);

        iterations++;
    }

    void makeCoin() {
        do {
            coin = new Coin();
        } while(sqrt((coin->x - x) * (coin->x - x) + (coin->y - y) * (coin->y - y)) < 2 * coin->radius + 2 * radius);
    }

    ~Hero() {
        delete coin;
    }
};

#endif