#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <vector>
#include <ctime>

const int framerate = 30;
const int window_width = 800;
const int window_height = 800;

class Circle {
public:
    const float vel = 5;
    float radius = 30;
    float x, y;
    bool horizontal;
    int orientation; // 0 = positive 1 = negative

    Circle() {
        // init its position
        horizontal = rand()%2;
        orientation = rand()%2;
        x = radius + rand()%int(window_width - 2 * radius);
        y = radius + rand()%int(window_height - 2 * radius);
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void move() {
        // horizontal circle
        if(horizontal) {
            if(orientation == 0) {
                x += vel;
                if(x + radius >= window_width) {
                    x -= (x + radius - (window_width - 1)); // kickback
                    orientation = 1 - orientation;
                }
            } else {
                x -= vel;
                if(x - radius < 0) {
                    x += 0 - (x - radius); // kickback
                    orientation = 1 - orientation;
                }
            }
        } else { // vertical circle
            if(orientation == 0) {
                y += vel;
                if(y + radius >= window_height) {
                    y -= (y + radius - (window_height - 1)); // kickback
                    orientation = 1 - orientation;
                }
            }  else {
                y -= vel;
                if(y - radius < 0) { // kickback
                    y += 0 - (y - radius);
                    orientation = 1 - orientation;
                }
            }
        }
    }

    ~Circle() {
        // does nothing
    }
};


class Game {
public:
    std::vector<Circle> circles;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_TIMER * game_loop_timer;
    ALLEGRO_TIMER * circle_timer;
    ALLEGRO_EVENT_QUEUE * event_queue;
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
        game_loop_timer = al_create_timer(1.0/framerate); // 30fps
        circle_timer = al_create_timer(2.0);
        event_queue = al_create_event_queue();

        // fonts
        font = al_load_ttf_font("./arial.ttf", 26, 0);
        
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_timer_event_source(game_loop_timer));
        al_register_event_source(event_queue, al_get_timer_event_source(circle_timer));
    }

    void begin() {
        al_start_timer(game_loop_timer);
        al_start_timer(circle_timer);
        game_loop();
    }

    void game_loop() {
        while(1) {
            // tres passos:
            // ler eventos
            // fazer a logica do jogo
            // renderizar

            // no allegro, a parte de renderizacao tambem eh um evento, eh o evento do timer que setamos pra renderizar
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);

            if(event.type == ALLEGRO_EVENT_TIMER and event.timer.source == game_loop_timer) {
                game_logic();
                render();
            } else if(event.type == ALLEGRO_EVENT_TIMER and event.timer.source == circle_timer) {
                Circle c = Circle();
                circles.push_back(c);
            } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
        }
    }

    void game_logic() {
        for(Circle & c : circles) {
            c.move();
        }
    }

    void render() {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        //al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Hello world!");
        
        for(Circle c : circles) {
            al_draw_circle(c.x, c.y, c.radius, al_map_rgb(0, 0, 0), 5);
            al_draw_filled_circle(c.x, c.y, c.radius, al_map_rgb(0, 0, 255));
        }
        
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

void init() {
    srand(time(0));

    

    al_init(); // first thing you should do
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
