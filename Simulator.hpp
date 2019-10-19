#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

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

#include "constants.h"
#include "Circle.hpp"
#include "Hero.hpp"

class Simulator {
public:
    const int newCircleInterval = 300; // every 10 seconds, if humans play the game with 30fps
    bool shouldRender;
    std::vector<Circle> circles;
    std::vector<Hero *> population;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_FONT * font;
    
    void init() {

        if (al_get_display_option(display, ALLEGRO_SAMPLE_BUFFERS)) {
            printf("With multisampling, level %d\n", al_get_display_option(display, ALLEGRO_SAMPLES));
            al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
            al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
        }
        else {
            printf("Without multisampling.\n");
        }

        display = al_create_display(window_width, window_height);

        // fonts
        font = al_load_ttf_font("./arial.ttf", 26, 0);
    }

    bool colliding(Hero * h) {
        for(Circle c : circles) {
            if(c.active == false) continue;
            float distance = sqrt((c.x - h->x) * (c.x - h->x) + (c.y - h->y) * (c.y - h->y));
            if(distance <= c.radius + h->radius) 
                return true;
        }
        return false;
    }

    void simulate(std::vector<Hero *> population) {
        this->population = population;
        game_loop();
    }

    void game_loop() {
        int untilNewCircle = 0;
        while(population.size()) {

            untilNewCircle++;
            if(untilNewCircle == newCircleInterval) {
                Circle c = Circle();
                circles.push_back(c);
                untilNewCircle = 0;
            }

            game_logic();

            if(shouldRender) {
                render();
                al_rest(0.0001); // wait a little bit
            } 
        }
    }

    void game_logic() {
        for(Circle & c : circles) {
            c.move();
        }

        for(int i = 0; i < population.size(); ++i) {
            Hero * h = population[i];
            if(colliding(h)) {
                population.erase(population.begin() + i); // erases and shifts left all elements to the right
                i--; // thats why we need to decrement i
                continue;
            }
            h->iterations++;
            
            // What is missing here is to capture and offer the input of the hero's neural network
            // Then get the result of the neural network and use it to move the hero

            // h->applyForce(result of neural network)
            // h->move
            
            if(h->gotCoin()) {
                h->makeCoin();
                h->score++;
            }
        }
    }

    void render() {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        //al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Hello world!");
        
        // render circles
        for(Circle c : circles) {
            al_draw_circle(c.x, c.y, c.radius, al_map_rgba(0, 0, 0, c.active? 255 : 20), 5);
            al_draw_filled_circle(c.x, c.y, c.radius, al_map_rgba(0, 0, 255, c.active? 255 : 20));
        }

        // render heros
        for(Hero * h : population) {
            al_draw_circle(h->x, h->y, h->radius, al_map_rgb(0, 0, 0), 5);
            if(h->collided) al_draw_filled_circle(h->x, h->y, h->radius, al_map_rgb(255, 0, 0));
            else al_draw_filled_circle(h->x, h->y, h->radius, al_map_rgb(0, 255, 0));
        
            // render hero's coin
            al_draw_circle(h->coin->x, h->coin->y, h->coin->radius, al_map_rgb(0, 0, 0), 3);
            al_draw_filled_circle(h->coin->x, h->coin->y, h->coin->radius, al_map_rgb(255, 255, 0));
        }
        

        // flip display
        al_flip_display();
    }

    ~Simulator() {
        al_destroy_font(font);
        al_destroy_display(display);
    }
};

#endif