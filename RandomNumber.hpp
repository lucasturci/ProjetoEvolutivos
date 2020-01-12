/*
Developers:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

#ifndef RANDOM_NUMBER_HPP
#define RANDOM_NUMBER_HPP

#include <cstdlib>
#include <chrono>
#include <ctime>
#include <random>
#include <climits>

using std::mt19937;

class RandomNumber {
public:
    mt19937 rng;
    RandomNumber() {
        rng = mt19937(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    RandomNumber(int seed) {
        rng = mt19937(seed);
    }

    mt19937 get_rng() {
        return rng;
    }

    double randDouble(double l, double r) {
        std::uniform_real_distribution<double> ur(l, r);
        return ur(rng); 
    }

    float randDouble(float l, float r) {
        std::uniform_real_distribution<float> ur(l, r);
        return ur(rng);
    }

    int randInt(int l = INT_MIN, int r = INT_MAX) {
        std::uniform_int_distribution<int> ui(l, r);
        return ui(rng);
    }

    static RandomNumber * getGenerator() {
        static RandomNumber * gen = NULL;

        if(!gen) {
            gen = new RandomNumber();
            return gen;
        }
    }
};

#endif