#ifndef EVENT_HPP
#define EVENT_HPP

#include "SDL_events.h"
#include <functional>
#include <map>

namespace nebula::events {

struct Event {
    Event(SDL_Event &e);
    SDL_Event sdl;
    bool consummated;
};

using Callback = std::function<bool(SDL_Event *)>;
using CallbackList = std::vector<Callback>;
using CallbackMap = std::map<SDL_EventType, CallbackList>; 

class EventManager {
public:
    EventManager();
    ~EventManager();
    void processEvent(SDL_Event *event);
    template <typename F>
    void registerEventCallback(const SDL_EventType type, F callback) {
        if (!m_callbacks.contains(type)) {
            m_callbacks[type] = CallbackList();
        }
        m_callbacks[type].emplace_back(callback);
    }
private:
    std::map<SDL_EventType, std::vector<Callback>> m_callbacks;
};

inline EventManager eventManager;

}

#endif//EVENT_HPP
