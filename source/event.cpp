#include "SDL_events.h"
#include <algorithm>
#include <event.hpp>
#include <functional>
#include <ranges>

using namespace nebula::events;

EventManager::EventManager() {

}

EventManager::~EventManager() {

}

bool EventManager::processEvent(SDL_Event *event) {
    SDL_EventType type = (SDL_EventType) event->type;
    if (!m_callbacks.contains(type)) {
        return false;
    }
    std::vector<Callback> &callbacks = m_callbacks.at(type);
    for (const auto &callback : callbacks) {
        bool consummated = callback(event);
        if (consummated) return true;
    }
    std::vector<SDLEventCallbackPtr> &callback_ptrs = m_callback_pointers.at(type);
    for (const auto &callback : callback_ptrs) {
        bool consummated = callback(event);
        if (consummated) return true;
    }
    return false;
}

void EventManager::registerEventCallback(const SDL_EventType type, const std::function<bool(SDL_Event*)> &callback) {
    m_callbacks[type].emplace_back(callback);
}
    
void EventManager::registerEventCallbackPtr(const SDL_EventType type, SDLEventCallbackPtr callback) {
    m_callback_pointers[type].emplace_back(callback);
}

void register_event_callback(uint16_t type, SDLEventCallbackPtr callback) {
    eventManager.registerEventCallbackPtr((SDL_EventType) type, callback); 
}
