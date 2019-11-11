// Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// C++
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// Game
#include "constants.h"
#include "Coin.hpp"
#include "Circle.hpp"
#include "Hero.hpp"
#include "Game.hpp"
#include "Simulator.hpp"
#include "RandomNumber.hpp"

bool render = false;

void init_allegro() {
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

std::string to_file_path = "";
std::string from_file_path = "";

Hero * selectParent(vector<Hero * > population) {
    RandomNumber * gen = RandomNumber::getGenerator();
    int n = population.size();
    int m = gen->randInt(1, n);
    int x = gen->randInt(0, m-1);
    return population[x];
}

void evolve(int n = 1000) {
    const int mutation_rate = 0.1;
    RandomNumber * gen = RandomNumber::getGenerator();

    // select types to check if user pressed Ctrl+D
    fd_set rfds;
    struct timeval tv;

    std::ifstream file;
    if(from_file_path.size()) {
        file.open(from_file_path, std::ios::in | std::ios::binary);
        if(!file.is_open()) {
            printf("Could not open file %s\n", from_file_path.c_str());
            printf("Evolving with random individuals instead\n");
        }
    }
    
    // generate population
    vector<Hero *> population(n);
    for(Hero *& h : population) {
        h = new Hero();
        if(from_file_path.size()) { 
            h->net->readFromFile(file);
        }
    }

    Simulator sim(render);

    std::pair<double, double> lastScore = {-1, -1};
    printf("Initing evolution, with population size: %d\n", n);
    printf("Press Ctrl+D (EOF) to stop\n");
    int generation = 0;
    while(1) {
        printf("Simulating generation %d, with %d individuals\n", generation, n);
        sim.init();
        int ret = sim.simulate(population);
   
        // sort hero's by fitness
        // fitness is the score of the hero, and ties are decided by the number of changes in positions
        std::sort(population.begin(), population.end(), [](Hero * h1, Hero * h2) {
            if(h1->score == h2->score) return h1->distance_to_coin < h2->distance_to_coin;
            return h1->score > h2->score;
        });

        std::pair<double, double> score = {population[0]->score, population[0]->distance_to_coin};

        printf("Best individual: score = %d, distance_to_coin = %.3lf\n", population[0]->score, population[0]->distance_to_coin);

        if(ret == 1) {
            printf("Terminating evolution\n");
            break;
        }

        tv.tv_sec = 0, tv.tv_usec = 1;
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        int aux = select(1, &rfds, NULL, NULL, &tv);
        if(aux == -1) {
            perror("select()");
        } else if(aux) {
            int x = fgetc(stdin);
            if(x == EOF) {
                printf("Terminating evolution\n");
                break;
            }
        }

        // Primeiros 40% fazem crossover -> gera 40%
        // Proximos 40% fazem cria dois a dois -> gera 20%
        // MVP faz cria com outros 20 % -> gera 20%
        // Restantes 20 % sofrem genocidio

        for(int i = 0; i < n; ++i) {
            population[i]->clear();
        }

        vector<Hero * > vec(population.begin(), population.end());
        std::cout << 1 << std::endl;
        // Pega um cara bom e cria um igual
        for(int i = 1; i < 0.4 * n; i++) {
            population[i]->setNet(*selectParent(vec)->net);
        }

        std::cout << 2 << std::endl;
        // Cria dois a dois nos proximos 30%
        for(int i = 0.4 * n, cur = 0.4 * n; i + 1 < n; i += 2, cur++) {
            NeuralNet net = NeuralNet::mix(*population[i]->net, *population[i+1]->net);
            population[cur]->setNet(net);
        }

        std::cout << 3 << std::endl;
        // mixa com o best
        for(int i = 0.7 * n; i < n; ++i) {
            NeuralNet net = NeuralNet::mix(*population[0]->net, *population[i]->net);
            population[i]->setNet(net);
        }

        std::cout << 4 << std::endl;
        for(int i = 0.25 * n; lastScore == score and generation%10 == 0 and i < n; ++i) {
            population[i]->net->randomizeNet();
        }

        std::cout << 5 << std::endl;
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
                *addresses[j] += gen->randDouble(-20.0, 20.0);
            }
        }

        lastScore = score;
        generation++;
    }

    if(to_file_path.size()) {
        printf("Saving to file\n");
        std::ofstream file;
        file.open(to_file_path, std::ios::out | std::ios::binary | std::ios::trunc);

        if(file.is_open()) {
            for(int i = 0; i < n; ++i) {
                population[i]->net->writeToFile(file);
            }
            file.close();
        } else {
            printf("Could not open file with path: %s\n", to_file_path.c_str());
        }
    }

    for(int i = 0; i < n; ++i) delete population[i];
}


void simulateGame(int n = 1) {
    
    std::ifstream file;
    if(from_file_path.size()) {
        file.open(from_file_path, std::ios::in | std::ios::binary);
        if(!file.is_open()) {
            printf("Could not open file %s\n", from_file_path.c_str());
            printf("Simulating with random individuals instead\n");
        }
    }
    vector<Hero * > population(n);
    for(Hero *& h : population) {
        h = new Hero();
        if(from_file_path.size()) { 
            h->net->readFromFile(file);
        }
    }
    
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

    int pop_size = -1;
    for(int i = 2; i < argc; ++i) {
        std::string flag = argv[i];
        if(flag.size() >= 2 and flag[0] == '-' and flag[1] == '-') {
            flag = flag.substr(2);
            if(flag == "render") render = true;
            if(flag.substr(0, 3) == "to=") {
                to_file_path = flag.substr(3);
            }
            if(flag.substr(0, 5) == "from=") {
                from_file_path = flag.substr(5);
            }
            if(flag.substr(0, 2) == "n=") {
                sscanf(flag.substr(2).c_str(), "%d", &pop_size);
            }
        }
    }

    if(std::string(argv[1]) == "game") render = true;
    if(render) init_allegro();
    
    if(std::string(argv[1]) == "simulate") {
        simulateGame(pop_size < 0? 1 : pop_size);
    } else if(std::string(argv[1]) == "game") {
        Game game;

        game.init();
        game.begin();
    } else if(std::string(argv[1]) == "evolve") { 
        if(pop_size > 0) evolve(pop_size);
        else evolve();
    } else {
        printf("Unknown command: %s\n", argv[1]);

    }

    
    if(render) shutdown_allegro();

    return 0;
}
