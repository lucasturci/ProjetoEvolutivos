#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const int window_width = 800;
const int window_height = 800;

int main() {

    al_init(); // first thing you should do
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY * display = al_create_display(window_width, window_height);
    ALLEGRO_TIMER * timer = al_create_timer(1.0/30);
    ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
    // fonts
    ALLEGRO_FONT * font = al_load_ttf_font("./arial.ttf", 26, 0);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    
    bool redraw = false;
    while(1) {
        // tres passos:
        // ler eventos
        // fazer a logica do jogo
        // renderizar

        // no allegro, a parte de renderizacao tambem eh um evento, eh o evento do timer que setamos pra renderizar
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Hello world!");
            al_flip_display();
        } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}
