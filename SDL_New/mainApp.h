#ifndef _MAINAPP_H_
    #define _MAINAPP_H_

    #include <SDL2/SDL.h>

    class mainApp{
        private:
            bool Running;
            SDL_Window* Surf_Display;

        public:
            mainApp();
            int onExecute();

        public:
            bool OnInit();
            void OnEvent(SDL_Event* Event);
            void OnLoop();
            void OnRender();
            void OnCleanup();
    };
#endif // _MAINAPP_H_
