#include <iostream>
#include <SDL2/SDL.h>
#include "../render/SDLBackend.h"
#include "../core/SimulatedDataSource.h"
#include "../core/Layout.h"
#include "../instruments/AttitudeIndicator.h"

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
    SimulatedDataSource dataSource;
    AttitudeIndicator attitudeIndicator;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    Rectangle region = computeAttitudeIndicatorRegion(windowWidth, windowHeight);


    Uint32 lastTicks = SDL_GetTicks();
    while (running) {
        Uint32 currentTicks = SDL_GetTicks();
        double dt = (currentTicks - lastTicks) / 1000.0;
        lastTicks = currentTicks;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        FlightState state = dataSource.update(dt);
        DrawList drawList = attitudeIndicator.render(state, region);
        backend.render(drawList);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
