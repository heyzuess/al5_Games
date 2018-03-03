#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

using namespace std;

const int SCREEN_WIDTH    = 640;
const int SCREEN_HEIGHT   = 480;
const int SCREEN_DURATION = 5;

void logSDLError(std::ostream &os, const std::string &errMsg);
void renderTexture(SDL_Texture *t_renTex, SDL_Renderer *r_destRen, int iip_x, int iip_y);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *r_buffer);

int main(int argc, char* args [])
{
    SDL_Window   *w_default;
    SDL_Renderer *r_render;
    SDL_Texture  *t_foregrnd;
    SDL_Texture  *t_backgrnd;
    std::string c_imagePath;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(std::cout, "SDL_Init");
        return -1;
    }

    w_default = SDL_CreateWindow("Hello World!",100,100,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(w_default == NULL){
        logSDLError(std::cout, "SDL_CreateWindow");
        SDL_Quit();
        return -1;
    }

    r_render = SDL_CreateRenderer(w_default,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(r_render == NULL){
        logSDLError(std::cout, "SDL_CreateRenderer");
        SDL_Quit();
        return -1;
    }

    t_foregrnd = loadTexture("./snorlax.bmp", r_render);
    t_backgrnd = loadTexture("./loadImage.bmp", r_render);
    if(t_foregrnd == NULL || t_backgrnd == NULL){
        logSDLError(std::cout, "Texture_Creation");
        SDL_DestroyTexture(t_foregrnd);
        SDL_DestroyTexture(t_backgrnd);
        SDL_DestroyRenderer(r_render);
        SDL_DestroyWindow(w_default);
        SDL_Quit();
        return -1;
    }

    for(int i = 0; i < SCREEN_DURATION; ++i){
        SDL_RenderClear(r_render);

        int i_bw, i_bh, i_fw, i_fh, i_x, i_y;
        SDL_QueryTexture(t_backgrnd, NULL, NULL, &i_bw, &i_bh);
        i_x = SCREEN_WIDTH - i_bw;
        i_y = SCREEN_HEIGHT - i_bh;
        renderTexture(t_backgrnd, r_render, 0, 0);
        renderTexture(t_backgrnd, r_render, i_x, 0);
        renderTexture(t_backgrnd, r_render, 0, i_y);
        renderTexture(t_backgrnd, r_render, i_x, i_y);

        SDL_QueryTexture(t_foregrnd, NULL, NULL, &i_fw, &i_fh);
        i_x = SCREEN_WIDTH / 2 - i_fw / 2;
        i_y = SCREEN_HEIGHT / 2 - i_fh / 2;
        renderTexture(t_foregrnd, r_render, i_x, i_y);

        SDL_RenderPresent(r_render);
        SDL_Delay(1000);
    }

    SDL_DestroyTexture(t_foregrnd);
    SDL_DestroyTexture(t_backgrnd);
    SDL_DestroyRenderer(r_render);
    SDL_DestroyWindow(w_default);
    SDL_Quit();

    return 0;
}

void logSDLError(std::ostream &os, const std::string &errMsg){
    os << errMsg << " error: " << SDL_GetError() << std::endl;
}

void renderTexture(SDL_Texture *t_renTex, SDL_Renderer *r_destRen, int iip_x, int iip_y){
    SDL_Rect r_destRect;
    r_destRect.x = iip_x;
    r_destRect.y = iip_y;
    SDL_QueryTexture(t_renTex, NULL, NULL, &r_destRect.w, &r_destRect.h);
    SDL_RenderCopy(r_destRen, t_renTex, NULL, &r_destRect);
}

SDL_Texture* loadTexture(const std::string &c_loadFile, SDL_Renderer *r_buffer){
    SDL_Texture *t_buffText = NULL;
    SDL_Surface *b_loadImage;

    b_loadImage = SDL_LoadBMP(c_loadFile.c_str());
    if(b_loadImage != NULL){
        t_buffText = SDL_CreateTextureFromSurface(r_buffer, b_loadImage);
        SDL_FreeSurface(b_loadImage);
        if(t_buffText == NULL){
            logSDLError(std::cout,"SDL_CreateTextureFromSurface");
        }
    }
    else{
        logSDLError(std::cout, "SDL_LoadBMP");
    }

    return t_buffText;
}
