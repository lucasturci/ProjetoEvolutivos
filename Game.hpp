/*
Desenvolvedores:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

#ifndef GAME_HPP
#define GAME_HPP

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

class Game {
public:
    std::vector<Circle> circles;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_TIMER * game_loop_timer;
    ALLEGRO_TIMER * circle_timer;
    ALLEGRO_EVENT_QUEUE * event_queue;
    ALLEGRO_FONT * font;
    Hero hero;
    
    void init() {

        display = al_create_display(window_width, window_height);
        
        if (al_get_display_option(display, ALLEGRO_SAMPLE_BUFFERS)) {
            printf("With multisampling, level %d\n", al_get_display_option(display, ALLEGRO_SAMPLES));
            al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
            al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
        }
        else {
            printf("Without multisampling.\n");
        }

        game_loop_timer = al_create_timer(1.0/framerate); // 30fps
        circle_timer = al_create_timer(2.0);
        event_queue = al_create_event_queue();

        // fonts
        font = al_load_ttf_font("./arial.ttf", 26, 0);
        
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_timer_event_source(game_loop_timer));
        al_register_event_source(event_queue, al_get_timer_event_source(circle_timer));
        al_register_event_source(event_queue, al_get_keyboard_event_source());
    }

    void begin() {
        al_start_timer(game_loop_timer);
        al_start_timer(circle_timer);
        game_loop();
    }

    bool colliding(Hero & h) {
        for(Circle c : circles) {
            if(c.active == false) continue;
            float distance = sqrt((c.x - h.x) * (c.x - h.x) + (c.y - h.y) * (c.y - h.y));
            if(distance <= c.radius + h.radius) 
                return true;
        }
        return false;
    }

    void game_loop() {
        bool should_render = false;
        int movex, movey;
        movex = movey = 0;

        while(1) {
            // tres passos:
            // ler eventos
            // fazer a logica do jogo
            // renderizar

            // no allegro, a parte de renderizacao tambem eh um evento, eh o evento do timer que setamos pra renderizar
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_TIMER and event.timer.source == game_loop_timer) should_render = true;

            if(event.type == ALLEGRO_EVENT_TIMER and event.timer.source == circle_timer) {
                Circle c = Circle();
                circles.push_back(c);

            } if(event.type == ALLEGRO_EVENT_KEY_DOWN) {

                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT) movex++;
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT) movex--;
                if(event.keyboard.keycode == ALLEGRO_KEY_UP) movey--;
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) movey++;

            } else if(event.type == ALLEGRO_EVENT_KEY_UP) {

                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT) movex--;
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT) movex++;
                if(event.keyboard.keycode == ALLEGRO_KEY_UP) movey++;
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) movey--;

            } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }

            if(al_is_event_queue_empty(event_queue) and should_render) {
                game_logic(movex, movey);
                render();
                should_render = false;
            }
        }
    }

    void game_logic(int movex, int movey) {
        for(Circle & c : circles) {
            c.move();
        }

        hero.collided = hero.collided or colliding(hero);

        hero.applyForce(movex, movey);
        hero.move();

        if(hero.gotCoin()) {
            hero.makeCoin();
            hero.score++;
        }
    }

    void render() {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        
        char score_string[67];
        sprintf(score_string, "%d", hero.score);
        al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, 40, ALLEGRO_ALIGN_CENTER, score_string);
        
        // render circles
        for(Circle c : circles) {
            al_draw_circle(c.x, c.y, c.radius, al_map_rgba(0, 0, 0, c.active? 255 : 20), 5);
            al_draw_filled_circle(c.x, c.y, c.radius, al_map_rgba(0, 0, 255, c.active? 255 : 20));
        }

        // render hero
        al_draw_circle(hero.x, hero.y, hero.radius, al_map_rgb(0, 0, 0), 5);
        if(hero.collided) al_draw_filled_circle(hero.x, hero.y, hero.radius, al_map_rgb(255, 0, 0));
        else al_draw_filled_circle(hero.x, hero.y, hero.radius, al_map_rgb(0, 255, 0));
        
        // render hero's coin
        al_draw_circle(hero.coin->x, hero.coin->y, hero.coin->radius, al_map_rgb(0, 0, 0), 3);
        al_draw_filled_circle(hero.coin->x, hero.coin->y, hero.coin->radius, al_map_rgb(255, 255, 0));

        // flip display
        al_flip_display();
    }

    ~Game() {
        al_destroy_font(font);
        al_destroy_display(display);
        al_destroy_timer(game_loop_timer);
        al_destroy_timer(circle_timer);
        al_destroy_event_queue(event_queue);
    }
};

#endif