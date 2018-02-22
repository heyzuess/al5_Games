#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

int mainChar(int argc, char **argv){
    ALLEGRO_DISPLAY *d_charDisp = NULL;
    char *cv_message = "";

    if(!al_init()){
        cv_message = "Failed to initialize Allegro5!\n";
        fprintf(stderr, cv_message);
        return -1;
    }

    d_charDisp = al_create_display(800,600);
    if(!d_charDisp){
        cv_message = "Failed to initialize Display!\n";
        fprintf(stderr, cv_message);
        return -1;
    }

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    al_rest(5.0);

    al_destroy_display(d_charDisp);
    return 0;
}
