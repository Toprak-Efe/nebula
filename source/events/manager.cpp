#include <functional>
#include "../../include/nebula/events/manager.hpp"

using namespace nebula::events;

EventManager::~EventManager() {

}

bool EventManager::processEvent(Event *event) {
    EventType type = event->type;
    if (!m_callbacks.contains(type)) {
        return false;
    }
    std::vector<Callback> &callbacks = m_callbacks.at(type);
    for (const auto &callback : callbacks) {
        bool consummated = callback(event);
        if (consummated) return true;
    }
    return false;
}

void EventManager::registerEventCallback(const EventType type, const std::function<bool(Event*)> &callback) {
    m_callbacks[type].emplace_back(callback);
}
    
EventManager &EventManager::get() {
    static EventManager eventManager;
    return eventManager;
}
