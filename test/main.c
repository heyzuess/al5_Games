#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BOUNCER_SIZE = 16;
enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main(int argc, char **argv)
{
 ALLEGRO_DISPLAY      *display = NULL;
 ALLEGRO_EVENT_QUEUE  *event_queue = NULL;
 ALLEGRO_TIMER        *timer = NULL;
 ALLEGRO_BITMAP       *bouncer = NULL;
 float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
 float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
 float bouncer_dx = -4.0, bouncer_dy = 4.0;
 bool key[4] = { false, false, false, false};
 bool redraw = true;
 bool freedom = true;
 bool doexit = false;
 char* cv_error = "";

 if(!al_init()){
     cv_error = "Could not initialize Allegro 5!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     return -1;
 }

 if(!al_init_image_addon()){
     cv_error = "Failed to initialize al_init_image_addon!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     return -1;
 }

 if(!al_install_mouse()){
     cv_error = "Failed to initialize mouse input!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     return -1;
 }

 if(!al_install_keyboard()){
     cv_error = "Failed to initialize keyboard input!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     return -1;
 }

 timer = al_create_timer(1.0 / FPS);
 if(!timer) {
     cv_error = "Failed to create timer!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     return -1;
 }

 display = al_create_display(SCREEN_W, SCREEN_H);
 if(!display){
     cv_error = "Could not create Allegro 5 display\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     return -1;
 }

 bouncer = al_load_bitmap("loadImage.png");
 if(!bouncer){
     cv_error = "failed to load bouncer image bitmap!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);

     al_destroy_display(display);
     al_destroy_timer(timer);
     return -1;
 }

 /* Removed since this will clear bitmap image */
 //al_set_target_bitmap(bouncer);
 //al_clear_to_color(al_map_rgb(255,0,255));
 //al_set_target_bitmap(al_get_backbuffer(display));

 event_queue = al_create_event_queue();
 if(!event_queue){
     cv_error = "Failed to create event_queue!\n";
     fprintf(stderr, cv_error);
     al_show_native_message_box(display, "Error", "Error", cv_error,
                                NULL, ALLEGRO_MESSAGEBOX_ERROR);
     al_destroy_bitmap(bouncer);
     al_destroy_display(display);
     al_destroy_timer(timer);
     return -1;
 }

 al_register_event_source(event_queue, al_get_display_event_source(display));
 al_register_event_source(event_queue, al_get_timer_event_source(timer));
 al_register_event_source(event_queue, al_get_mouse_event_source());
 al_register_event_source(event_queue, al_get_keyboard_event_source());
 al_clear_to_color(al_map_rgb(0,0,0));
 al_flip_display();
 al_start_timer(timer);

 while(!doexit){
   ALLEGRO_EVENT ev;
   al_wait_for_event(event_queue, &ev);

   if(ev.type == ALLEGRO_EVENT_TIMER){
       if(freedom){
           if(bouncer_x < 0 || bouncer_x > SCREEN_W - BOUNCER_SIZE){
               bouncer_dx = -bouncer_dx;
           }

           if(bouncer_y < 0 || bouncer_y > SCREEN_H - BOUNCER_SIZE){
               bouncer_dy = -bouncer_dy;
           }

           bouncer_x += bouncer_dx;
           bouncer_y += bouncer_dy;
       }
       else{
           if(key[KEY_UP] && bouncer_y >= 4.0){
               bouncer_y -= 4.0;
           }
           if(key[KEY_DOWN] && bouncer_y <= SCREEN_H - BOUNCER_SIZE - 4.0){
               bouncer_y += 4.0;
           }
           if(key[KEY_LEFT] && bouncer_x >= 4.0){
               bouncer_x -= 4.0;
           }
           if(key[KEY_RIGHT] && bouncer_x <= SCREEN_W - BOUNCER_SIZE - 4.0){
               bouncer_x += 4.0;
           }
       }

       redraw = true;
   }
   else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
       fprintf(stdout, "Application has been closed.\n");
       break;
   }
   else if((ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
            ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) &&
            !freedom){
       bouncer_x = ev.mouse.x;
       bouncer_y = ev.mouse.y;
   }
   else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
       if(freedom)
           freedom = false;
       else
           freedom = true;
   }
   else if(ev.type == ALLEGRO_EVENT_KEY_DOWN &&
           !freedom){
       switch(ev.keyboard.keycode){
           case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               break;
           case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               break;
           case ALLEGRO_KEY_LEFT:
               key[KEY_LEFT] = true;
               break;
           case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;
               break;
       }
   }
   else if(ev.type == ALLEGRO_EVENT_KEY_UP &&
           !freedom){
       switch(ev.keyboard.keycode){
           case ALLEGRO_KEY_UP:
                key[KEY_UP] = false;
                break;
           case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = false;
                break;
           case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = false;
                break;
           case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = false;
                break;
           case ALLEGRO_KEY_ESCAPE:
                doexit = true;
                break;
       }
   }

   if(redraw && al_is_event_queue_empty(event_queue)){
       redraw = false;

       al_clear_to_color(al_map_rgb(0,0,0));
       al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
       al_flip_display();
   }
 }

 al_destroy_bitmap(bouncer);
 al_destroy_timer(timer);
 al_destroy_display(display);
 al_destroy_event_queue(event_queue);

 return 0;
}
