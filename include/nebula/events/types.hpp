#pragma once

namespace nebula::events {

enum class EventType {
    MissingEvent,
    WindowClose,
    KeyPressed,
    KeyReleased,
    MouseMoved,
    MouseButtonPressed,
    MouseButtonReleased
}; // enum class EventType

struct Event {
    EventType type;

    struct KeyboardEvent {
        int key_code;
        bool is_repeat;
    };

    struct MouseButtonEvent {
        int button;
        int clicks;
        int x, y;
    };

    struct MouseMotionEvent {
        int x, y;
        int x_rel, y_rel;
    };

    union {
        KeyboardEvent keyboard;
        MouseButtonEvent mouse_button;
        MouseMotionEvent mouse_motion;
    };
}; // struct Event

} // namespace nebula::Events
