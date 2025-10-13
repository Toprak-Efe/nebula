#include <cassert>
#include <SDL2/SDL_events.h>
#include "../../include/nebula/events/conversion.hpp"

nebula::events::Event nebula::events::sdl_to_nebula(SDL_Event &event) {
    Event out;
    switch (event.type) {
        case SDL_QUIT:
            out.type = nebula::events::EventType::WindowClose;
        case SDL_KEYDOWN:
            out.type = nebula::events::EventType::KeyPressed;
        case SDL_KEYUP:
            out.type = nebula::events::EventType::KeyReleased;
        case SDL_MOUSEBUTTONDOWN:
            out.type = nebula::events::EventType::MouseButtonPressed;
        case SDL_MOUSEBUTTONUP:
            out.type = nebula::events::EventType::MouseButtonReleased;
        case SDL_MOUSEMOTION: 
            out.type = nebula::events::EventType::MouseMoved;
        default:
            assert(false && "Unhandled SDL event.");
            break;
    }
    return out;
}

