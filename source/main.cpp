#include "ecs/components/transform.hpp"
#include "ecs/ecsmanager.hpp"
#include "glm/fwd.hpp"
#include <atomic>
#include <ratio>

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

namespace astronomy {

using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
using millisecond = std::chrono::duration<uint64_t, std::milli>;
using microsecond = std::chrono::duration<uint64_t, std::micro>;
using clock = std::chrono::high_resolution_clock;

} // namespace astronomy
  
using namespace astronomy;
using namespace std::literals::chrono_literals;

int main() {
    rendering::manager.initialize();
    resources::meshManager.initialize();
    resources::shaderManager.initialize();
    resources::textureManager.initialize();

    {
        const flecs::world &world = data::ecsManager.getRegistry(); 
        flecs::entity default_cube = world.entity("default_cube");
        default_cube.add<data::Transform>();
        default_cube.add<data::Program>();
        default_cube.set<data::Program>({"starShader"});
        default_cube.add<data::Mesh>();
        default_cube.set<data::Mesh>({"cube"});
        flecs::entity default_camera = world.entity("default_camera");
        default_camera.add<data::Camera>();
        default_camera.set<data::Camera>({1920, 1080, 60.0});
        default_camera.add<data::Active>();
        default_camera.add<data::Transform>();
        default_camera.get_mut<data::Transform>().position.x = -1.0;
    }

    constexpr size_t TICKS_PER_SEC = 20;
    constexpr auto TIME_PER_TICK = 1000ms / TICKS_PER_SEC;

    timepoint t_last = clock::now();
    millisecond t_accumulated{0}; 
    
    std::atomic_bool running = true;
    events::eventManager.registerEventCallback(SDL_QUIT,
        [&](SDL_Event *e) { return (bool) (running = false); }
    );
    while (running) {
        millisecond delta = std::chrono::duration_cast<millisecond> (clock::now() - t_last);
        t_last += delta;
        t_accumulated += delta;

        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            ImGui_ImplSDL2_ProcessEvent(&sdl_event);
            events::eventManager.processEvent(&sdl_event);
        }

        while (t_accumulated > TIME_PER_TICK) {
            t_accumulated -= TIME_PER_TICK;
            data::ecsManager.progressSystems(TIME_PER_TICK.count());
        }
        
        rendering::manager.drawAll();
    }

    logger.log<INFO>("Exiting application.");
    exit(0);
}
