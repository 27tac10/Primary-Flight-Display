#ifndef SDL_BACKEND_H
#define SDL_BACKEND_H

#include "Backend.h"
#include <SDL2/SDL.h>
#include <vector>

class SDLBackend : public Backend {
    public:
        SDLBackend(SDL_Renderer* r);
        void render(const DrawList& r) override;
        
    private:
        SDL_Renderer* renderer;
        std::vector<SDL_Rect> clipStack;

        void renderCommand(const LineCommand& cmd);
        void renderCommand(const FilledPolygonCommand& cmd);
        void renderCommand(const TextCommand& cmd);
        void renderCommand(const PushClipCommand& cmd);
        void renderCommand(const PopClipCommand& cmd);
        void renderCommand(const PushTransformCommand& cmd);
        void renderCommand(const PopTransformCommand& cmd);
};

#endif // SDL_BACKEND_H