// SDL2 Hello, World!
// This should display a white screen for 2 seconds
// compile with: clang++ main.cpp -o hello_sdl2 -lSDL2
// run with: ./hello_sdl2
#include <SDL2/SDL.h>
#include <stdio.h>
#include "mainApp.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

mainApp::mainApp(){
    Surf_Display = NULL;
    Running = true;
}

int mainApp::onExecute(){
    if(OnInit() == false){
        return -1;
    }

    SDL_Event Event;

    while(Running){
        while(SDL_PollEvent(&Event)){
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();
    return 0;
}

int main(int argc, char* args[]) {
  mainApp app_CM;
  return app_CM.onExecute();
}
