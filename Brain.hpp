/*
Developers:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <vector>
#include <random>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "RandomNumber.hpp"

using std::vector;

class Brain {

public:
    vector<vector<double> > mat;

    int N;
    Brain(){} // default constructor
    
    // N: number of parameters in input
    // number of parameters in output is 4
    Brain(int N) {
        mat = vector<vector<double> > (4, vector<double> (N));
        this->N = N;
        randomize();            
    }

    void randomize() {
        RandomNumber * gen = RandomNumber::getGenerator();
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < N; ++j) {
                mat[i][j] = gen->randDouble(-1000.0, 1000.0);
            }
        }
    }

    void print() {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < N; ++j) {
                std::cout << mat[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    vector<double> calculate(vector<double> I) {
        if(I.size() != N) throw std::runtime_error("ERROR: Input size is different than brain size");

        vector<double> ans(4);

        for(int i = 0; i < 4; ++i) {
            ans[i] = 0.0;
            for(int j = 0; j < N; ++j) {
                ans[i] += mat[i][j] * I[j];
            }
        }
        return ans;
    }

    void mutate(double mutation_rate) {
        RandomNumber * gen = RandomNumber::getGenerator();
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < N; ++j) {
                if(gen->randInt(0, 1)) mat[i][j] += gen->randDouble(-mutation_rate, mutation_rate);
            }
        }
    }

    void writeToFile(std::ofstream & file) {

        file.write((char *) &N, sizeof(N));
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < N; ++j) {
                file.write((char *) &mat[i][j], sizeof (mat[i][j]));
            }
        }
    }

    void readFromFile(std::ifstream & file) {
        file.read((char *) &N, sizeof(N));
        mat.assign(4, vector<double>(N));
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < N; ++j) {
                file.read((char *) &mat[i][j], sizeof (mat[i][j]));
            }
        }
    }

    static Brain mix(Brain a, Brain b) {
        Brain ret(a);
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < b.N; ++j) {
                ret.mat[i][j] += b.mat[i][j];
                ret.mat[i][j] /= 2;
            }
        }
        return ret;
    }

    static void crossover(Brain & a, Brain & b) {
        RandomNumber * gen = RandomNumber::getGenerator();
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < a.N; ++j) {
                if(gen->randInt(0, 1)) {
                    std::swap(a.mat[i][j], b.mat[i][j]);
                }
            }
        }
    }

};

#endif