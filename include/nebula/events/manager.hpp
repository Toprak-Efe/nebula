#pragma once

#include <map>
#include <functional>
#include "types.hpp"

namespace nebula::events {

using Callback = std::function<bool(Event *)>;

class EventManager {
public:
    ~EventManager();
    EventManager(const EventManager&) = delete;
    EventManager(EventManager&&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    static EventManager &get();
    bool processEvent(Event *event);
    void registerEventCallback(const EventType type, const Callback &callback);
private:
    EventManager();
    std::map<EventType, std::vector<Callback>> m_callbacks;
};

}

