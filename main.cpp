#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <ctime>

const int framerate = 30;
const int window_width = 800;
const int window_height = 800;

class Circle {
public:
    const float vel = 5;
    float x, y;
    bool horizontal;
    int orientation; // 0 = positive 1 = negative

    Circle() {
        // init its position
        horizontal = rand()%2;
        orientation = rand()%2;
        x = rand()%window_width;
        y = rand()%window_height;
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
                if(x >= window_width) x -= (x - (window_width - 1)); // kickback
            } else {
                x -= vel;
                if(x < 0) x += 0 - x; // kickback
            }
        } else { // vertical circle
            if(orientation == 0) {
                y += vel;
                if(y >= window_height) y -= (y - (window_height - 1)); // kickback
            }  else {
                y -= vel;
                if(y < 0) y += 0 - y;
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
    ALLEGRO_TIMER * timer;
    ALLEGRO_EVENT_QUEUE * queue;
    ALLEGRO_FONT * font;
    
    void init() {
        display = al_create_display(window_width, window_height);
        timer = al_create_timer(1.0/framerate); // 30fps
        queue = al_create_event_queue();

        // fonts
        font = al_load_ttf_font("./arial.ttf", 26, 0);
        
        al_register_event_source(queue, al_get_display_event_source(display));
        al_register_event_source(queue, al_get_timer_event_source(timer));
    }

    void begin() {
        al_start_timer(timer);

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
            al_wait_for_event(queue, &event);

            if(event.type == ALLEGRO_EVENT_TIMER) {
                game_logic();
                render();
                
            } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
        }
    }

    void game_logic() {

    }

    void render() {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Hello world!");
        al_flip_display();
    }

    ~Game() {
        al_destroy_font(font);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
    }
};

void init() {
    srand(time(0));

    al_init(); // first thing you should do
    al_init_font_addon();
    al_init_ttf_addon();
}


int main() {

    init();
    Game game;

    game.init();
    game.begin();

    return 0;
}
