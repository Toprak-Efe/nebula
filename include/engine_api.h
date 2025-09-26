#ifndef ENGINE_API_H
#define ENGINE_API_H

#include <flecs.h>
#include <cstdint>

union SDL_Event;

struct EngineAPI {
    flecs::world* world;
    void (*log_info)(const char* fmt, ...);
    void (*log_debug)(const char* fmt, ...);
    void (*log_error)(const char* fmt, ...);

    void (*event_register_callback)(uint32_t event_type, bool (*callback)(SDL_Event* e));
};

struct GameAPI {
    void (*setup)(const EngineAPI* engine_api);
    void (*update)(float delta_time);
    void (*shutdown)();
};

#endif//ENGINE_API_H
