#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include "constants.h"
#include "Coin.hpp"
#include "Circle.hpp"
#include "Hero.hpp"
#include "Game.hpp"

void init() {
    srand(time(0));

    

    al_init(); // first thing you should do
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
}

void shutdown() {

    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
}


int main() {

    init();
    Game game;

    game.init();
    game.begin();

    shutdown();

    return 0;
}
