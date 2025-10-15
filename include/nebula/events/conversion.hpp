#pragma once

#include "types.hpp"
#include <SDL2/SDL_events.h>

namespace nebula::events {

inline
void toNebula(const SDL_Event &sdl_event, Event& nebula_event) {
    switch (sdl_event.type) {
        case SDL_QUIT:
            nebula_event.type = nebula::events::EventType::WindowClose;
            break;
        case SDL_KEYDOWN:
            nebula_event.type = nebula::events::EventType::KeyPressed;
            nebula_event.keyboard.key_code = sdl_event.key.keysym.sym;
            break;
        case SDL_KEYUP:
            nebula_event.type = nebula::events::EventType::KeyReleased;
            nebula_event.keyboard.key_code = sdl_event.key.keysym.sym;
            break;
        case SDL_MOUSEBUTTONDOWN:
            nebula_event.type = nebula::events::EventType::MouseButtonPressed;
            nebula_event.mouse_button.button = sdl_event.button.button;
            nebula_event.mouse_button.clicks = sdl_event.button.clicks;
            nebula_event.mouse_button.x = sdl_event.button.x;
            nebula_event.mouse_button.y = sdl_event.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_MOUSEMOTION: 
            nebula_event.type = nebula::events::EventType::MouseMoved;
            nebula_event.mouse_motion.x = sdl_event.motion.x;
            nebula_event.mouse_motion.x_rel = sdl_event.motion.xrel;
            nebula_event.mouse_motion.y = sdl_event.motion.y;
            nebula_event.mouse_motion.y_rel = sdl_event.motion.yrel;
            break;
        default:
            nebula_event.type = nebula::events::EventType::MissingEvent;
            break;
    }
}

inline
void fromNebula(const Event& nebula_event, SDL_Event &sdl_event) {
    // TO-DO
}

} // namepsace nebula::data

