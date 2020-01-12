/*
Developers:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

#ifndef HERO_HPP
#define HERO_HPP

#include "constants.h"
#include "Coin.hpp"
#include "Brain.hpp"

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
    double distance_to_circle;
    int changes;
    Coin * coin;
    Brain * brain;
    vector<double> distances;
    const int seed = 302;
    RandomNumber * gen;

    Hero() {
        x = window_width/2;
        y = window_height/2; 
        vx = vy = 0;
        collided = false;
        changes = 0;
        score = 0;
        coin = NULL;
        distance_to_circle = 1000.0;

        gen = new RandomNumber(seed);
        makeCoin();

        // Input layer: sensors, distance in x to coin, distance in y to coin, velocity in x and velocity in y
        brain = new Brain(alpha + 4);
    }

    // reset this guy, but keeps the brain
    void clear() {
        x = window_width/2;
        y = window_height/2; 
        vx = vy = 0;
        collided = false;
        changes = 0;
        score = 0;
        gen = new RandomNumber(seed);
        distance_to_circle = 1000.0;
        makeCoin();
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    // use linear combination to decide what move to make in this iteration of the game, based on the sensors passed as input
    // d is a vector of distances until a ball or a wall. If it is a wall, maybe we can increase it a little bit just to discourage avoiding the walls
    vector<int> decide(vector<double> d) {
        vector<double> I = d;
        I.push_back(vx);
        I.push_back(vy);
        I.push_back(coin->x - x); // dx: delta x between coin and hero positions
        I.push_back(coin->y - y); // dy: delta y between coin and hero positions

        vector<double> dec = brain->calculate(I);
        vector<int> ret;
        for(int x : dec) {
            ret.push_back(x > 0);
        }
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

    void setBrain(Brain brain) {
        if(this->brain) delete this->brain;
        this->brain = new Brain(brain);
    }

    void makeCoin() {
        if(coin) delete coin;
        coin = new Coin(gen->randInt(Coin::radius, window_width - Coin::radius), gen->randInt(Coin::radius, window_height - Coin::radius));
    }

    double fitness() {
        return 100000.0 * score + 10 * distance_to_circle - distance_to_coin;
    }

    ~Hero() {
        delete coin;
        delete brain;
    }
};

#endif