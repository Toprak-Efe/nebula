#pragma once

#include "types.hpp"
#include <SDL2/SDL_events.h>

namespace nebula::events {

Event sdl_to_nebula(SDL_Event &event);

} // namepsace nebula::data

