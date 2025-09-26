#include "SDL_events.h"
#include <algorithm>
#include <event.hpp>
#include <functional>
#include <ranges>

using namespace nebula::events;

Event::Event(SDL_Event &e) : 
    sdl(e),
    consummated(false)
{}

EventManager::EventManager() {

}

EventManager::~EventManager() {

}

void EventManager::processEvent(SDL_Event *event) {
    SDL_EventType type = (SDL_EventType) event->type;
    if (!m_callbacks.contains(type)) {
        return;
    }
    std::vector<Callback> &callbacks = m_callbacks.at(type);
    for (const auto &callback : callbacks) {
        bool consummated = callback(event);
        if (consummated) return;
    }
}

