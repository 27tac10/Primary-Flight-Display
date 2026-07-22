#include "SDLBackend.h"

SDLBackend::SDLBackend(SDL_Renderer* r) {
    renderer = r;
}

void SDLBackend::render(const DrawList& r) {
    for (const auto& command : r) {
        if (std::holds_alternative<LineCommand>(command)) {
            renderCommand(std::get<LineCommand>(command));
        } else if (std::holds_alternative<FilledPolygonCommand>(command)) {
            renderCommand(std::get<FilledPolygonCommand>(command));
        } else if (std::holds_alternative<TextCommand>(command)) {
            renderCommand(std::get<TextCommand>(command));
        } else if (std::holds_alternative<PushClipCommand>(command)) {
            renderCommand(std::get<PushClipCommand>(command));
        } else if (std::holds_alternative<PopClipCommand>(command)) {
            renderCommand(std::get<PopClipCommand>(command));
        } else if (std::holds_alternative<PushTransformCommand>(command)) {
            renderCommand(std::get<PushTransformCommand>(command));
        } else if (std::holds_alternative<PopTransformCommand>(command)) {
            renderCommand(std::get<PopTransformCommand>(command));
        }
    }
}

void SDLBackend::renderCommand(const LineCommand& cmd) {
    // Implement rendering logic for LineCommand using SDL
}

void SDLBackend::renderCommand(const FilledPolygonCommand& cmd) {
    // Implement rendering logic for FilledPolygonCommand using SDL
}

void SDLBackend::renderCommand(const TextCommand& cmd) {
    // Implement rendering logic for TextCommand using SDL
}

void SDLBackend::renderCommand(const PushClipCommand& cmd) {
    // Implement rendering logic for PushClipCommand using SDL
    SDL_Rect clipRect;
    clipRect.x = static_cast<int>(cmd.clipRect.x);
    clipRect.y = static_cast<int>(cmd.clipRect.y);
    clipRect.w = static_cast<int>(cmd.clipRect.width);
    clipRect.h = static_cast<int>(cmd.clipRect.height);

    clipStack.push_back(clipRect);
    SDL_RenderSetClipRect(renderer, &clipStack.back());
}

void SDLBackend::renderCommand(const PopClipCommand& cmd) {
    // Implement rendering logic for PopClipCommand using SDL
    if (clipStack.empty()) {
        return; // No clip to pop
    }

    clipStack.pop_back();

    if (clipStack.empty()) {
        SDL_RenderSetClipRect(renderer, nullptr); // No clip
    } else {
        SDL_RenderSetClipRect(renderer, &clipStack.back());
    }
}

void SDLBackend::renderCommand(const PushTransformCommand& cmd) {
    // Implement rendering logic for PushTransformCommand using SDL
}

void SDLBackend::renderCommand(const PopTransformCommand& cmd) {
    // Implement rendering logic for PopTransformCommand using SDL
}