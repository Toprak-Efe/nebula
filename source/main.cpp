#include <glm/fwd.hpp>
#include <atomic>
#include <cstdlib>
#include <iterator>
#include <ratio>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_events.h>
#include <GL/gl.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <flecs.h>

#include "../generated/nebula/version.hpp"
#include "../include/nebula/ecs/ecs.hpp"
#include "../include/nebula/resources/shaders.hpp"
#include "../include/nebula/resources/meshes.hpp"
#include "../include/nebula/render/manager.hpp"
#include "../include/nebula/render/types.hpp"
#include "../include/nebula/utils/log.hpp"
#include "../include/nebula/events/manager.hpp"
#include "../include/nebula/events/types.hpp"
#include "../include/nebula/events/conversion.hpp"
#include "../include/nebula/nebula.hpp"

namespace nebula {

    using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using millisecond = std::chrono::duration<uint64_t, std::milli>;
    using microsecond = std::chrono::duration<uint64_t, std::micro>;
    using clock = std::chrono::high_resolution_clock;

} // namespace nebula

using namespace std::literals::chrono_literals;
using namespace nebula;

int main() {
    Logger &logger = Logger::get();
    logger.log<Levels::INFO> ("ENTERING {}", version::PROJECT_NAME);
    logger.log<Levels::INFO> ("{} VERSION {}", version::PROJECT_NAME, version::PROJECT_VERSION);
    logger.log<Levels::INFO> ("{} BUILD DATE {}", version::PROJECT_NAME, version::PROJECT_BUILD_DATE);

    data::ECSManager &ecsManager = data::ECSManager::get();
    events::EventManager &eventManager = events::EventManager::get();
    rendering::RenderingManager &renderManager = rendering::RenderingManager::get();
    resources::ShaderManager &shaderManager = resources::ShaderManager::get();
    resources::MeshManager &meshManager = resources::MeshManager::get();
    
    ecsManager.initialize(); 
    renderManager.initialize();
    shaderManager.initialize();
    meshManager.initialize();

    NebulaApi nebulaApi {
        .events = &eventManager,
        .ecs = &ecsManager
    };
    game::initialize(nebulaApi);
    
    constexpr size_t TICKS_PER_SEC = 20;
    constexpr auto TIME_PER_TICK = 1000ms / TICKS_PER_SEC;

    timepoint t_last = clock::now();
    millisecond t_accumulated{0}; 

    std::atomic_bool running = true;
    //eventManager.registerEventCallback(events::EventType::WindowClose,
    //     [&](events::Event *) -> bool {
    //        return false;
    //    }
    //);
    while (running) {
        millisecond delta = std::chrono::duration_cast<millisecond> (clock::now() - t_last);
        t_last += delta;
        t_accumulated += delta;

        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            ImGui_ImplSDL2_ProcessEvent(&sdl_event);
            if (sdl_event.type == SDL_QUIT) running = false;
            //events::Event event = events::sdl_to_nebula(sdl_event); 
            //eventManager.processEvent(&event);
        }

        while (t_accumulated > TIME_PER_TICK) {
            t_accumulated -= TIME_PER_TICK;
            float delta_time = TIME_PER_TICK.count()/1000.0f;
            ecsManager.progressSystems(delta_time);
        }

        renderManager.drawAll();
    }

    logger.log<Levels::INFO>("Exiting application.");
    exit(0);
}

