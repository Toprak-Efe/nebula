#include <ecs/components/transform.hpp>
#include <ecs/components/velocity.hpp>
#include <ecs/ecsmanager.hpp>
#include <glm/fwd.hpp>
#include <atomic>
#include <cstdlib>
#include <random>
#include <ratio>
#include <chrono>

#include <SDL_events.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <flecs.h>

#include <ecs/ecs.hpp>
#include <log.hpp>
#include <event.hpp>
#include <textures.hpp>
#include <window.hpp>
#include <shaders.hpp>
#include <rendering.hpp>
#include <game/game.hpp>

namespace nebula {

    using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using millisecond = std::chrono::duration<uint64_t, std::milli>;
    using microsecond = std::chrono::duration<uint64_t, std::micro>;
    using clock = std::chrono::high_resolution_clock;

} // namespace nebula

using namespace std::literals::chrono_literals;
using namespace nebula;

int main() {
    data::ecsManager.initialize(); 
    rendering::manager.initialize();
    resources::meshManager.initialize();
    resources::shaderManager.initialize();
    resources::textureManager.initialize();
    
    nebula::game::setup();

    constexpr size_t TICKS_PER_SEC = 20;
    constexpr auto TIME_PER_TICK = 1000ms / TICKS_PER_SEC;

    timepoint t_last = clock::now();
    millisecond t_accumulated{0}; 

    std::atomic_bool running = true;
    events::eventManager.registerEventCallback(SDL_QUIT,
        [&](SDL_Event *) -> bool { return !((bool) (running = false)); }
    );
    while (running) {
        millisecond delta = std::chrono::duration_cast<millisecond> (clock::now() - t_last);
        t_last += delta;
        t_accumulated += delta;

        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            if (!events::eventManager.processEvent(&sdl_event)) {
                ImGui_ImplSDL2_ProcessEvent(&sdl_event);
            }
        }

        while (t_accumulated > TIME_PER_TICK) {
            t_accumulated -= TIME_PER_TICK;
            float delta_time = TIME_PER_TICK.count()/1000.0f;
            data::ecsManager.progressSystems(delta_time);
        }

        rendering::manager.drawAll();
    }

    logger.log<INFO>("Exiting application.");
    exit(0);
}
