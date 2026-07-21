#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main() {

    bool running = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

    if (!window) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }
        SDL_Surface *windowSurface = SDL_GetWindowSurface(window);

        if (!windowSurface) {
            cout << "Failed to get window surface: " << SDL_GetError() << endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }


        SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 255)); // Fill the window with blue color

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16); // Delay to limit frame rate (approx. 60 FPS)
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
