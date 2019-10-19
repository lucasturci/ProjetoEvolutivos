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

// Game
#include "constants.h"
#include "Coin.hpp"
#include "Circle.hpp"
#include "Hero.hpp"
#include "Game.hpp"
#include "Simulator.hpp"

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


int main(int argc, char * argv[]) {
    srand(time(0));
    if(argc == 1) {
        printf("Usage: %s evolve | game <flags>\n", argv[0]);
        return 0;
    }

    if(render) init_allegro();
    
    if(std::string(argv[1]) == "evolve") {

    } else {
        Game game;

        game.init();
        game.begin();
    }

    
    if(render) shutdown_allegro();

    return 0;
}
