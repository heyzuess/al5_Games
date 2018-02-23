#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#define FIRE_AMMO 4
#define NUM_KEYS  5
const int SCREEN_W    = 800;
const int SCREEN_H    = 600;
const int FPS         = 60;
const int CHAR_SIZE   = 16;
const int FIRE_DIST   = 50;
const float STEP_SIZE = 4.0;
enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, SPACE
};
enum AMMO {
    BULLET_1, BULLET_2, BULLET_3, BULLET_4
};

int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *d_mainDisp = NULL;
    ALLEGRO_EVENT_QUEUE *e_defaultEQ = NULL;
    ALLEGRO_TIMER *t_defaultTime;
    ALLEGRO_BITMAP *b_mainActor;
    char *cv_message = "Failed to initialize ";
    bool lv_exit = false;
    bool lv_redraw = false;
    bool lv_init[FIRE_AMMO] = {false,false,false,false};
    bool lv_drawCircle[FIRE_AMMO] = {false, false, false, false};
    bool lv_keys[NUM_KEYS] = {false, false, false, false, false};
    float actor_x   = SCREEN_W / CHAR_SIZE + CHAR_SIZE;
    float actor_y   = SCREEN_H / 2.0 - CHAR_SIZE / 2.0;
    float circle_ix = actor_x + CHAR_SIZE;
    float circle_iy = actor_y + STEP_SIZE * 2;
    float circle_x[FIRE_AMMO] = {circle_ix,circle_ix,circle_ix,circle_ix};
    float circle_y[FIRE_AMMO] = {circle_iy,circle_iy,circle_iy,circle_iy};
    float circle_dx = STEP_SIZE, circle_dy; //, circle_dy = STEP_SIZE;
    int64_t currentTime[FIRE_AMMO];
    int64_t passTime[FIRE_AMMO];

    if(!al_init()){
        strcat(cv_message, "Allegro5!\n");
        fprintf(stderr,cv_message);
        return -1;
    }

    // Returns Void
    al_init_font_addon();

    if(!al_init_primitives_addon()){
        strcat(cv_message, "Primitives Addon!\n");
        fprintf(stderr,cv_message);
        return -1;
    }

    if(!al_init_ttf_addon()){
        strcat(cv_message, "TFF Addon!\n");
        fprintf(stderr,cv_message);
        return -1;
    }

    if(!al_install_mouse()){
        strcat(cv_message, "Mouse!\n");
        fprintf(stderr,cv_message);
        return -1;
    }

    if(!al_install_keyboard()){
        strcat(cv_message, "Keyboard!\n");
        fprintf(stderr,cv_message);
        return -1;
    }

    t_defaultTime = al_create_timer(1.0 / FPS);
    if(!t_defaultTime){
        strcat(cv_message, "Timer!\n");
        fprintf(stderr,cv_message);
        return -1;
    }

    d_mainDisp = al_create_display(SCREEN_W,SCREEN_H);
    if(!d_mainDisp){
        strcat(cv_message, "Display!\n");
        fprintf(stderr,cv_message);

        al_destroy_timer(t_defaultTime);
        return -1;
    }

    b_mainActor = al_create_bitmap(CHAR_SIZE, CHAR_SIZE);
    if(!b_mainActor){
        strcat(cv_message, "Main Actor!\n");
        fprintf(stderr, cv_message);

        al_destroy_timer(t_defaultTime);
        al_destroy_display(d_mainDisp);
        return -1;
    }

    al_set_target_bitmap(b_mainActor);
    al_clear_to_color(al_map_rgb(255,0,255));
    al_set_target_bitmap(al_get_backbuffer(d_mainDisp));

    e_defaultEQ = al_create_event_queue();
    if(!e_defaultEQ){
        strcat(cv_message, "Event Queue!\n");
        fprintf(stderr,cv_message);

        al_destroy_timer(t_defaultTime);
        al_destroy_display(d_mainDisp);
        al_destroy_bitmap(b_mainActor);
        return -1;
    }

    al_register_event_source(e_defaultEQ, al_get_display_event_source(d_mainDisp));
    al_register_event_source(e_defaultEQ, al_get_timer_event_source(t_defaultTime));
    al_register_event_source(e_defaultEQ, al_get_keyboard_event_source());
    al_register_event_source(e_defaultEQ, al_get_mouse_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(t_defaultTime);

    while(!lv_exit){
        ALLEGRO_EVENT e_currEvent;
        al_wait_for_event(e_defaultEQ, &e_currEvent);

        if(e_currEvent.type == ALLEGRO_EVENT_TIMER){
            if(lv_keys[KEY_LEFT] == true && actor_x >= STEP_SIZE){
                actor_x -= STEP_SIZE;
            }
            if(lv_keys[KEY_RIGHT] == true && actor_x <= SCREEN_W - CHAR_SIZE - STEP_SIZE){
                actor_x += STEP_SIZE;
            }
            if(lv_keys[KEY_UP] == true && actor_y >= STEP_SIZE){
                actor_y -= STEP_SIZE;
            }
            if(lv_keys[KEY_DOWN] == true && actor_y <= SCREEN_H - CHAR_SIZE - STEP_SIZE){
                actor_y += STEP_SIZE;
            }
            if(lv_keys[SPACE] == true){
                if(!lv_drawCircle[BULLET_1]){
                    lv_drawCircle[BULLET_1] = true;
                    lv_init[BULLET_1] = true;
                    passTime[BULLET_1] = al_get_timer_count(t_defaultTime);
                }
                else if(!lv_drawCircle[BULLET_2] &&
                        currentTime[BULLET_1] - passTime[BULLET_1] > FIRE_DIST - (FIRE_DIST / 2)){
                    lv_drawCircle[BULLET_2] = true;
                    lv_init[BULLET_2] = true;
                    passTime[BULLET_2] = al_get_timer_count(t_defaultTime);
                }
                else if(!lv_drawCircle[BULLET_3] &&
                        currentTime[BULLET_2] - passTime[BULLET_2] > FIRE_DIST - (FIRE_DIST / 2)){
                    lv_drawCircle[BULLET_3] = true;
                    lv_init[BULLET_3] = true;
                    passTime[BULLET_3] = al_get_timer_count(t_defaultTime);
                }
                else if(!lv_drawCircle[BULLET_4] &&
                        currentTime[BULLET_3] - passTime[BULLET_3] > FIRE_DIST - (FIRE_DIST / 2)){
                    lv_drawCircle[BULLET_4] = true;
                    lv_init[BULLET_4] = true;
                    passTime[BULLET_4] = al_get_timer_count(t_defaultTime);
                }
            }
            if(lv_drawCircle[BULLET_1]){
                if(circle_x[BULLET_1] < 0 || circle_x[BULLET_1] >= SCREEN_W - CHAR_SIZE - STEP_SIZE){
                    circle_dx = -circle_dx;
                }
                if(circle_y[BULLET_1] < 0 || circle_y[BULLET_1] >= SCREEN_H - CHAR_SIZE - STEP_SIZE){
                    circle_dy = -circle_dy;
                }

                circle_x[BULLET_1] += circle_dx;
                //circle_y[BULLET_1] += circle_dy;
            }
            if(lv_drawCircle[BULLET_2]){
                if(circle_x[BULLET_2] < 0 || circle_x[BULLET_2] >= SCREEN_W - CHAR_SIZE - STEP_SIZE){
                    circle_dx = -circle_dx;
                }
                if(circle_y[BULLET_2] < 0 || circle_y[BULLET_2] >= SCREEN_H - CHAR_SIZE - STEP_SIZE){
                    circle_dy = -circle_dy;
                }

                circle_x[BULLET_2] += circle_dx;
                //circle_y[BULLET_2] += circle_dy;
            }
            if(lv_drawCircle[BULLET_3]){
                if(circle_x[BULLET_3] < 0 || circle_x[BULLET_3] >= SCREEN_W - CHAR_SIZE - STEP_SIZE){
                    circle_dx = -circle_dx;
                }
                if(circle_y[BULLET_3] < 0 || circle_y[BULLET_3] >= SCREEN_H - CHAR_SIZE - STEP_SIZE){
                    circle_dy = -circle_dy;
                }

                circle_x[BULLET_3] += circle_dx;
                //circle_y[BULLET_3] += circle_dy;
            }
            if(lv_drawCircle[BULLET_4]){
                if(circle_x[BULLET_4] < 0 || circle_x[BULLET_4] >= SCREEN_W - CHAR_SIZE - STEP_SIZE){
                    circle_dx = -circle_dx;
                }
                if(circle_y[BULLET_4] < 0 || circle_y[BULLET_4] >= SCREEN_H - CHAR_SIZE - STEP_SIZE){
                    circle_dy = -circle_dy;
                }

                circle_x[BULLET_4] += circle_dx;
                //circle_y[BULLET_4] += circle_dy;
            }

            lv_redraw = true;
        }
        else if(e_currEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            lv_exit = true;
        }
        else if(e_currEvent.type == ALLEGRO_EVENT_MOUSE_AXES ||
                e_currEvent.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY){

        }
        else if(e_currEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){

        }
        else if(e_currEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

        }
        else if(e_currEvent.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(e_currEvent.keyboard.keycode){
                case ALLEGRO_KEY_LEFT:
                lv_keys[KEY_LEFT] = false;
                break;

                case ALLEGRO_KEY_RIGHT:
                lv_keys[KEY_RIGHT] = false;
                break;

                case ALLEGRO_KEY_DOWN:
                lv_keys[KEY_DOWN] = true;
                break;

                case ALLEGRO_KEY_UP:
                lv_keys[KEY_UP] = true;
                break;

                case ALLEGRO_KEY_SPACE:
                lv_keys[SPACE] = true;
                break;
            }
        }
        else if(e_currEvent.type == ALLEGRO_EVENT_KEY_UP){
            switch(e_currEvent.keyboard.keycode){
                case ALLEGRO_KEY_LEFT:
                lv_keys[KEY_LEFT] = false;
                break;

                case ALLEGRO_KEY_RIGHT:
                lv_keys[KEY_RIGHT] = false;
                break;

                case ALLEGRO_KEY_DOWN:
                lv_keys[KEY_DOWN] = false;
                break;

                case ALLEGRO_KEY_UP:
                lv_keys[KEY_UP] = false;
                break;

                case ALLEGRO_KEY_SPACE:
                lv_keys[SPACE] = false;
                break;

                case ALLEGRO_KEY_ESCAPE:
                lv_exit = true;
                break;
            }
        }

        if(lv_redraw && al_event_queue_is_empty(e_defaultEQ)){
            lv_redraw = false;

            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(b_mainActor, actor_x, actor_y, 0);

            circle_ix = actor_x + CHAR_SIZE;
            circle_iy = actor_y + STEP_SIZE * 2;
            if(lv_drawCircle[BULLET_1]){
                if(lv_init[BULLET_1]){
                    lv_init[BULLET_1] = false;
                    circle_x[BULLET_1] = circle_ix;
                    circle_y[BULLET_1] = circle_iy;
                }
                al_draw_filled_circle(circle_x[BULLET_1],circle_y[BULLET_1],STEP_SIZE,al_map_rgb(200,100,0));
                currentTime[BULLET_1] = al_get_timer_count(t_defaultTime);
                if(currentTime[BULLET_1] - passTime[BULLET_1] > FIRE_DIST){
                    lv_drawCircle[BULLET_1] = false;
                    circle_x[BULLET_1] = circle_ix;
                    circle_y[BULLET_1] = circle_iy;
                    circle_dx = STEP_SIZE;
                    //circle_dy = STEP_SIZE;
                }
            }
            if(lv_drawCircle[BULLET_2]){
                if(lv_init[BULLET_2]){
                    lv_init[BULLET_2] = false;
                    circle_x[BULLET_2] = circle_ix;
                    circle_y[BULLET_2] = circle_iy;
                }
                al_draw_filled_circle(circle_x[BULLET_2],circle_y[BULLET_2],STEP_SIZE,al_map_rgb(200,100,0));
                currentTime[BULLET_2] = al_get_timer_count(t_defaultTime);
                if(currentTime[BULLET_2] - passTime[BULLET_2] > FIRE_DIST){
                    lv_drawCircle[BULLET_2] = false;
                    circle_x[BULLET_2] = circle_ix;
                    circle_y[BULLET_2] = circle_iy;
                    circle_dx = STEP_SIZE;
                    //circle_dy = STEP_SIZE;
                }
            }
            if(lv_drawCircle[BULLET_3]){
                if(lv_init[BULLET_3]){
                    lv_init[BULLET_3] = false;
                    circle_x[BULLET_3] = circle_ix;
                    circle_y[BULLET_3] = circle_iy;
                }
                al_draw_filled_circle(circle_x[BULLET_3],circle_y[BULLET_3],STEP_SIZE,al_map_rgb(200,100,0));
                currentTime[BULLET_3] = al_get_timer_count(t_defaultTime);
                if(currentTime[BULLET_3] - passTime[BULLET_3] > FIRE_DIST){
                    lv_drawCircle[BULLET_3] = false;
                    circle_x[BULLET_3] = circle_ix;
                    circle_y[BULLET_3] = circle_iy;
                    circle_dx = STEP_SIZE;
                    //circle_dy = STEP_SIZE;
                }
            }
            if(lv_drawCircle[BULLET_4]){
                if(lv_init[BULLET_4]){
                    lv_init[BULLET_4] = false;
                    circle_x[BULLET_4] = circle_ix;
                    circle_y[BULLET_4] = circle_iy;
                }
                al_draw_filled_circle(circle_x[BULLET_4],circle_y[BULLET_4],STEP_SIZE,al_map_rgb(200,100,0));
                currentTime[BULLET_3] = al_get_timer_count(t_defaultTime);
                if(currentTime[BULLET_4] - passTime[BULLET_4] > FIRE_DIST){
                    lv_drawCircle[BULLET_4] = false;
                    circle_x[BULLET_4] = circle_ix;
                    circle_y[BULLET_4] = circle_iy;
                    circle_dx = STEP_SIZE;
                    //circle_dy = STEP_SIZE;
                }
            }
            al_flip_display();
        }
    }

    al_destroy_timer(t_defaultTime);
    al_destroy_display(d_mainDisp);
    al_destroy_bitmap(b_mainActor);
    al_destroy_event_queue(e_defaultEQ);
    return 0;
}
