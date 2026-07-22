#include <iostream>
#include <SDL2/SDL.h>
#include "../render/SDLBackend.h"

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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        cout << "SDL_CreateRender error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDLBackend backend(renderer);
    DrawList drawList;

    FilledPolygonCommand traiangle;
    traiangle.listOfPoints = {{300, 150}, {400, 350}, {200, 350}};
    traiangle.fillColor = {1.0, 0.0, 0.0, 1.0};
    drawList.push_back(traiangle);

    TextCommand label;
    label.text = "Hello PFD";
    label.position = {300, 100};
    label.anchor = TextAnchor::Center;
    label.fontSize = 32;
    label.color = {1.0, 1.0, 1.0, 1.0};
    drawList.push_back(label);

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        backend.render(drawList);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
