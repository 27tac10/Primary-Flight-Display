#include "SDLBackend.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

SDLBackend::SDLBackend(SDL_Renderer* r) {
    renderer = r;
    TTF_Init(); // Initialize SDL_ttf library
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
    Point start = applyTransform(cmd.start);
    Point end = applyTransform(cmd.end);
    Sint16 x1 = static_cast<Sint16>(start.x);
    Sint16 y1 = static_cast<Sint16>(start.y);
    Sint16 x2 = static_cast<Sint16>(end.x);
    Sint16 y2 = static_cast<Sint16>(end.y);
    Uint8 width = static_cast<Uint8>(cmd.width);
    Uint8 red = static_cast<Uint8>(cmd.color.r * 255);
    Uint8 green = static_cast<Uint8>(cmd.color.g * 255);
    Uint8 blue = static_cast<Uint8>(cmd.color.b * 255);
    Uint8 alpha = static_cast<Uint8>(cmd.color.a * 255);

    thickLineRGBA(renderer, x1, y1, x2, y2, width, red, green, blue, alpha);
}

void SDLBackend::renderCommand(const FilledPolygonCommand& cmd) {
    // Implement rendering logic for FilledPolygonCommand using SDL
    std::vector<Sint16> vx;
    std::vector<Sint16> vy;
    for (const auto& point : cmd.listOfPoints) {
        Point transformed = applyTransform(point);
        vx.push_back(static_cast<Sint16>(transformed.x));
        vy.push_back(static_cast<Sint16>(transformed.y));
    }

    Uint8 red = static_cast<Uint8>(cmd.fillColor.r * 255);
    Uint8 green = static_cast<Uint8>(cmd.fillColor.g * 255);
    Uint8 blue = static_cast<Uint8>(cmd.fillColor.b * 255);
    Uint8 alpha = static_cast<Uint8>(cmd.fillColor.a * 255);

    filledPolygonRGBA(renderer, vx.data(), vy.data(), static_cast<int>(vx.size()), red, green, blue, alpha);
}

void SDLBackend::renderCommand(const TextCommand& cmd) {
    // Implement rendering logic for TextCommand using SDL
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", static_cast<int>(cmd.fontSize));

    if (!font) {
        return; // could not load font
    }

    Uint8 red = static_cast<Uint8>(cmd.color.r * 255);
    Uint8 green = static_cast<Uint8>(cmd.color.g * 255);
    Uint8 blue = static_cast<Uint8>(cmd.color.b * 255);
    Uint8 alpha = static_cast<Uint8>(cmd.color.a * 255);

    SDL_Color textColor = { red, green, blue, alpha };

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, cmd.text.c_str(), textColor);
    if (!textSurface) {
        TTF_CloseFont(font);
        return; // could not create surface
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return; // could not create texture
    }

    SDL_Rect destRect;
    destRect.w = textSurface->w;
    destRect.h = textSurface->h;
    if (cmd.anchor == TextAnchor::TopLeft) {
        destRect.x = static_cast<int>(cmd.position.x);
        destRect.y = static_cast<int>(cmd.position.y);
    } else if (cmd.anchor == TextAnchor::Center) {
        destRect.x = static_cast<int>(cmd.position.x - destRect.w / 2);
        destRect.y = static_cast<int>(cmd.position.y - destRect.h / 2);
    }

    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
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
    transformStack.push_back(cmd);
}

void SDLBackend::renderCommand(const PopTransformCommand& cmd) {
    // Implement rendering logic for PopTransformCommand using SDL
    if (!transformStack.empty()) {
        transformStack.pop_back();
    }
}

Point SDLBackend::applyTransform(const Point& point) const{
    if (transformStack.empty()) {
        return point;
    }

    const Point& pivot = transformStack.back().pivot;
    double angleRadians = transformStack.back().rotationAngle * 3.14159265358979323846 / 180.0;

    double dx = point.x - pivot.x;
    double dy = point.y - pivot.y;

    Point result;
    result.x = pivot.x + dx * std::cos(angleRadians) - dy * std::sin(angleRadians);
    result.y = pivot.y + dx * std::sin(angleRadians) + dy * std::cos(angleRadians);

    return result;
}