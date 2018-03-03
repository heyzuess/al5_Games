#include "mainApp.h"

bool mainApp::OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0 ){
        return false;
    }

    Surf_Display = SDL_CreateWindow("Game Window",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    640,
                                    480,
                                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if(Surf_Display == NULL){
        return false;
    }

    return true;
}
