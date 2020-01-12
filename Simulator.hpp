/*
Developers:
    Clara Rosa (github.com/claraxsilveira)
    Lucas Turci (github.com/lucasturci)
    Raphael Medeiros Vieira (github.com/rmedeiros23)
*/

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

struct vec {
    double x, y;
    vec(){}
    vec(double a, double b): x(a), y(b) {}
    vec operator + (vec v) const {
        return vec(x + v.x, y + v.y);
    }
    vec operator - (vec v) const {
        return vec(x - v.x, y - v.y);
    }

    vec operator * (double a) const {
        return vec(x * a, y * a);
    }

    double operator * (vec v) const {
        return x * v.x + y * v.y;
    }


    double norm() {
        return sqrt(x * x + y * y);
    }

    static double dist(vec u, vec v) {
        return (u - v).norm();
    }
};

class Simulator {
public:
    const int newCircleInterval = 210; // every 7 seconds, if humans play the game with 30fps
    bool shouldRender;
    std::vector<Circle> circles;
    std::vector<Hero *> population;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_FONT * font;
    ALLEGRO_EVENT_QUEUE * event_queue;


    Simulator(bool render = false){
        shouldRender = render;
        display = NULL;
        font = NULL;
    }
    
    void init() {
        static bool sampled = false;
        circles.clear();
        Circle::setGenerator();
        
        if(shouldRender) {
            if(!display) {
                display = al_create_display(window_width, window_height);
                event_queue = al_create_event_queue();
                al_register_event_source(event_queue, al_get_display_event_source(display));
            }
            // fonts
            if(!font) font = al_load_ttf_font("./arial.ttf", 26, 0);

            if(!sampled) {
                if (al_get_display_option(display, ALLEGRO_SAMPLE_BUFFERS)) {
                    printf("With multisampling, level %d\n", al_get_display_option(display, ALLEGRO_SAMPLES));
                    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
                    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
                }
                else {
                    printf("Without multisampling.\n");
                }
                sampled = true;
            }
        }


    }

    bool colliding(Hero * h) {
        for(Circle c : circles) {
            if(c.active == false) continue;
            double distance = sqrt((c.x - h->x) * (c.x - h->x) + (c.y - h->y) * (c.y - h->y));
            h->distance_to_circle = std::min(h->distance_to_circle, distance);
            if(distance <= c.radius + h->radius) 
                return true;
        }
        return false;
    }

    // Returns 1 if user closed window, otherwise return 0
    int simulate(std::vector<Hero *> population) {
        this->population = population;
        return game_loop();
    }

    // Returns 1 if user closed window, otherwise returns 0
    int game_loop() {
        int untilNewCircle = 0;
        while(population.size()) {
            untilNewCircle++;
            if(untilNewCircle == newCircleInterval) {
                Circle c = Circle();
                if(circles.size() == 0) {
                    c.setPosition(window_width - c.radius, window_height/2);
                    c.horizontal = false;
                } else if(circles.size() == 1) {
                    c.setPosition(window_width/2, window_height - c.radius);
                    c.horizontal = true;
                } else if(circles.size() == 2) {
                    c.setPosition(c.radius, window_height/2);
                    c.horizontal = false;
                } else if(circles.size() == 3) {
                    c.setPosition(window_width/2, c.radius);
                    c.horizontal = true;
                }
                circles.push_back(c);
                untilNewCircle = 0;
            }

            if(shouldRender) {
                ALLEGRO_EVENT evt;
                while(al_get_next_event(event_queue, &evt)) {
                    if(evt.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 1;
                }
            }

            game_logic();

            if(shouldRender) {
                render();
                al_rest(0.0001); // wait a little bit
            } 
        }
        return 0;
    }

    // returns if circle centered in c with radius 'radius' intersects with half line segment starting at 'p' with direction vector 'dir'
    bool line_circle_intersection(vec p, vec dir, vec c, double radius) {
        vec intersec_point = p + dir * ((c - p) * dir);

        return vec::dist(c, intersec_point) <= radius and (intersec_point - p) * dir > 0;
    }

    void game_logic() {
        for(Circle & c : circles) {
            c.move();
        }

        for(int i = 0; i < population.size(); ++i) {
            Hero * h = population[i];
            if(colliding(h)) {
                h->distance_to_coin = sqrt((h->x - h->coin->x) * (h->x - h->coin->x) + (h->y - h->coin->y) * (h->y - h->coin->y));
                population.erase(population.begin() + i); // erases and shifts left all elements to the right
                i--; // thats why we need to decrement i
                continue;
            }

            // calculate distances in the direction of sensors
            const double pi = acos(-1);
            const double rate = 2.0 * pi / h->alpha;
            double angle = 0.0;

            vector<double> dist(h->alpha, 2000);

            const double eps = 1e-5;
            // calcula distances to walls
            for(int j = 0; j < h->alpha; ++j) {
                if(!(pi/2 <= angle and angle <= 3 * pi/2))
                    if(abs(cos(angle)) > eps) dist[j] = std::min(dist[j], (window_width - h->x)/cos(angle));

                if(0 <= angle and angle <= pi)
                    if(abs(sin(angle)) > eps) dist[j] = std::min(dist[j], (window_height - h->y)/sin(angle));
                
                if(pi/2 <= angle and angle <= 3 * pi/2)
                    if(abs(cos(angle)) > eps) dist[j] = std::min(dist[j], h->x/-cos(angle));
                
                if(pi <= angle and angle <= 2 * pi)
                    if(abs(sin(angle)) > eps) dist[j] = std::min(dist[j], h->y/-sin(angle));

                angle += rate;
            }

            angle = 0.0;
            for(int j = 0; j < h->alpha; ++j) {
                vec heroPosition = vec(h->x, h->y);
                vec v = vec(cos(angle), sin(angle)); // unitary vector with 'angle' angle between horizontal axis

                for(Circle c : circles) {
                    vec center = vec(c.x, c.y); // circle center
                    if(line_circle_intersection(heroPosition, v, center, c.radius)) {
                        vec p = heroPosition + v * ((center - heroPosition) * v);
                        double d = vec::dist(heroPosition, p) - sqrt(c.radius * c.radius - vec::dist(p, center) * vec::dist(p, center));
                        dist[j] = std::min(dist[j], d);
                    }
                }
                angle += rate;
            }

            h->distances = dist;

            // for(double x : dist) std::cout << x << ' ';
            // std::cout << std::endl;
            

            vector<int> decide = h->decide(dist);  // decide[0] = shouldMoveLeft, decide[1] = shouldMoveRight, decide[2] = shouldMoveUp, decide[3] = shouldMoveDown
            int dx = -1 * decide[0] + 1 * decide[1];
            int dy = -1 * decide[2] + 1 * decide[3];
            
            h->applyForce(dx, dy);
            h->move();
            
            if(h->gotCoin()) {
                h->makeCoin();
                h->score++;
            }
        }
    }

    void render() {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        //al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Hello world!");
        
        // render score 
        char score_string[67];
        sprintf(score_string, "%d", population[0]->score);
        al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, 40, ALLEGRO_ALIGN_CENTER, score_string);

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

            const double pi = acos(-1);
            // render hero's sensor lines
            for(int i = 0; i < h->alpha; ++i) {
                vec p = vec(h->x, h->y);
                vec q = vec(cos(i * (2 * pi/h->alpha)), sin(i * (2 * pi/h->alpha)));
                vec r = p + q * h->distances[i];
                al_draw_line(p.x, p.y, r.x, r.y, i < 4? al_map_rgb(255, 0, 0) : al_map_rgb(125, 167, 90), 1);
            }
        }
        

        // flip display
        al_flip_display();
    }

    ~Simulator() {
        if(shouldRender) {
            al_destroy_font(font);
            al_destroy_display(display);
        }
    }
};

#endif