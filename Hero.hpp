#ifndef HERO_HPP
#define HERO_HPP

#include "constants.h"
#include "Coin.hpp"
#include "NeuralNet.hpp"

#include <assert.h>

class Hero {
private:
public:
    const int alpha = 16; // number of sensors
    const float radius = 20.0;
    const float accel = .5;
    const float deaccel = .2;
    float x, y;
    float vx, vy;
    bool collided;
    int score;
    double distance_to_coin;
    int changes;
    Coin * coin;
    NeuralNet * net;
    vector<double> distances;

    Hero() {
        x = window_width/2;
        y = window_height/2; 
        vx = vy = 0;
        collided = false;
        changes = 0;
        score = 0;
        coin = NULL;
        makeCoin();

        // Input layer: sensors, distance in x to coin, distance in y to coin, velocity in x and velocity in y
        net = new NeuralNet(alpha + 4, 16, 4);
    }

    // reset this guy, but keeps the net
    void clear() {
        x = window_width/2;
        y = window_height/2; 
        vx = vy = 0;
        collided = false;
        changes = 0;
        score = 0;
        makeCoin();
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    // use neural network to decido what move to make in this iteration of the game, based on the sensors passed as input
    // d is a vector of distances until a ball or a wall. If it is a wall, maybe we can increase it a little bit just to discourage avoiding the walls
    vector<int> decide(vector<double> d) {
        vector<double> I = d;
        I.push_back(vx);
        I.push_back(vy);
        I.push_back(coin->x - x); // dx: delta x between coin and hero positions
        I.push_back(coin->y - y); // dy: delta y between coin and hero positions

        vector<double> dec = net->propagate(I);
        for(double & x : dec) 
            x = round(x);

        vector<int> ret(dec.begin(), dec.end());
        return ret;
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

        if(vx or vy) changes++;
        
    }

    void setNet(NeuralNet net) {
        if(this->net) delete this->net;
        this->net = new NeuralNet(net);
    }

    void makeCoin() {
        do {
            if(coin) delete coin;
            coin = new Coin();
        } while(sqrt((coin->x - x) * (coin->x - x) + (coin->y - y) * (coin->y - y)) < 2 * coin->radius + 2 * radius);
    }

    ~Hero() {
        delete coin;
        delete net;
    }
};

#endif