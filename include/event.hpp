#ifndef EVENT_HPP
#define EVENT_HPP

#include "SDL_events.h"
#include <functional>
#include <map>

typedef bool (*SDLEventCallbackPtr)(SDL_Event*);

namespace nebula::events {

using Callback = std::function<bool(SDL_Event *)>;

class EventManager {
public:
    EventManager();
    ~EventManager();
    bool processEvent(SDL_Event *event);
    void registerEventCallback(const SDL_EventType type, const Callback &callback);
    void registerEventCallbackPtr(const SDL_EventType type, SDLEventCallbackPtr callback);
private:
    std::map<SDL_EventType, std::vector<Callback>> m_callbacks;
    std::map<SDL_EventType, std::vector<SDLEventCallbackPtr>> m_callback_pointers;
};

inline EventManager eventManager;

}

extern "C" void register_event_callback(uint16_t type, SDLEventCallbackPtr callback); 

#endif//EVENT_HPP
