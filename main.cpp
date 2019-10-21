// Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// C++
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>

// Game
#include "constants.h"
#include "Coin.hpp"
#include "Circle.hpp"
#include "Hero.hpp"
#include "Game.hpp"
#include "Simulator.hpp"
#include "RandomNumber.hpp"

bool render = true;

void init_allegro() {
    srand(time(0));

    al_init(); // first thing you should do
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
}

void shutdown_allegro() {

    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
}

void evolve() {
    const int n = 100;
    const int mutation_rate = 0.1;
    RandomNumber * gen = RandomNumber::getGenerator();
    
    // generate population
    vector<Hero *> population(n);
    for(Hero *& h : population) 
        h = new Hero();

    Simulator sim(render);

    printf("Initing evolution, with population size: %d\n", n);
    printf("Enter 'exit' (without quotes) to stop\n");
    while(1) {
        // simulate game
        sim.init();
        sim.simulate(population);

        // sort hero's by fitness
        // fitness is the score of the hero, and ties are decided by the number of changes in positions
        std::sort(population.begin(), population.end(), [](Hero * h1, Hero * h2) {
            if(h1->score == h2->score) return h1->changes > h2->changes;
            return h1->score > h2->score;
        });

        // Primeiros 40% fazem crossover -> gera 40%
        // Proximos 40% fazem cria dois a dois -> gera 20%
        // MVP faz cria com outros 20 % -> gera 20%
        // Restantes 20 % sofrem genocidio

        for(int i = 0; i < n; ++i) {
            population[i]->clear();
        }

        // Crossover nos primeiros 40%
        for(int i = 1; i < 0.4 * n; i += 2) {
            NeuralNet::crossover( *population[i]->net, *population[i+1]->net);
        }

        // Cria dois a dois nos proximos 20%
        for(int i = 0.4 * n, cur = 0.4 * n; i < 0.8 * n; i += 2, cur++) {
            NeuralNet net = NeuralNet::mix(*population[i]->net, *population[i+1]->net);
            population[cur]->setNet(net);
        }
        for(int i = 0.6 * n; i < 0.8 * n; ++i) {
            NeuralNet net = NeuralNet::mix(*population[0]->net, *population[i]->net);
            population[i]->setNet(net);
        }

        for(int i = 0.8 * n; i < n; ++i) {
            population[i]->net->randomizeNet();
        }

        // mutate
        for(int i = 1; i < n; ++i) {
            Hero * h = population[i];
            vector<double *> addresses;
            for(int m = 0; m < h->net->M; ++m) {
                for(int l = 0; l < h->net->L + 1; l++) {
                    addresses.push_back(&h->net->Mh[m][l]);
                }
            }

            for(int k = 0; k < h->net->K; ++k) {
                for(int m = 0; m < h->net->M; ++m) {
                    addresses.push_back(&h->net->Mo[k][m]);
                }
            }
            std::shuffle(addresses.begin(), addresses.end(), gen->get_rng());
            for(int j = 0; j < (int) mutation_rate * addresses.size(); ++j) {
                *addresses[j] += gen->randDouble(-1.0, 1.0);
            }
        }
    }
}


void simulateGame(int n = 4) {
    vector<Hero * > population(n);
    for(Hero *& h : population) 
        h = new Hero();
    
    Simulator sim(render);

    sim.init();
    sim.simulate(population);
}


int main(int argc, char * argv[]) {
    srand(time(0));
    if(argc == 1) {
        printf("Usage: %s evolve | game | simulate <flags>\n", argv[0]);
        return 0;
    }

    if(std::string(argv[1]) == "game") render = true;
    if(render) init_allegro();
    
    if(std::string(argv[1]) == "simulate") {
        simulateGame();
    } else if(std::string(argv[1]) == "game") {
        Game game;

        game.init();
        game.begin();
    } else if(std::string(argv[1]) == "evolve") { 
        evolve();
    } else {
        printf("Unknown command: %s\n", argv[1]);

    }

    
    if(render) shutdown_allegro();

    return 0;
}
