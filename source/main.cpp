#include <engine_api.h>

#include "ecs/components/transform.hpp"
#include "ecs/ecsmanager.hpp"
#include "glm/fwd.hpp"
#include <atomic>
#include <cstdlib>
#include <random>
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

#ifdef _WIN32
    #include <windows.h>
    typedef HMODULE LibHandle;
    #define LOAD_LIBRARY(path) LoadLibrary(path)
    #define GET_FUNCTION(lib, name) GetProcAddress(lib, name)
    #define FREE_LIBRARY(lib) FreeLibrary(lib)
#else
    #include <dlfcn.h>
    typedef void* LibHandle;
    #define LOAD_LIBRARY(path) dlopen(path, RTLD_NOW)
    #define GET_FUNCTION(lib, name) dlsym(lib, name)
    #define FREE_LIBRARY(lib) dlclose(lib)
#endif

namespace nebula {

    using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using millisecond = std::chrono::duration<uint64_t, std::milli>;
    using microsecond = std::chrono::duration<uint64_t, std::micro>;
    using clock = std::chrono::high_resolution_clock;

} // namespace nebula

using namespace std::literals::chrono_literals;
using namespace nebula;


int main() {
    const flecs::world &world = data::ecsManager.getRegistry(); 
    rendering::manager.initialize();
    resources::meshManager.initialize();
    resources::shaderManager.initialize();
    resources::textureManager.initialize();

    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-5.0, 5.0);
        for (size_t i = 0; i < 1000; i++) {
            flecs::entity cube = world.entity();
            float p_x = dis(gen), p_y = dis(gen), p_z = dis(gen);
            float s_x = dis(gen), s_y = dis(gen), s_z = dis(gen);
            cube.add<data::Transform>();
            data::Transform &t = cube.get_mut<data::Transform>();
            t.position.x = p_x;
            t.position.y = p_y;
            t.position.z = p_z;
            t.scale.x = s_x;
            t.scale.y = s_y;
            t.scale.z = s_z;
            cube.add<data::Program>();
            cube.set<data::Program>({"starShader"});
            cube.add<data::Mesh>();
            cube.set<data::Mesh>({"cube"});
        }
        flecs::entity default_cube = world.entity("default_cube");
        default_cube.add<data::Transform>();
        default_cube.add<data::Program>();
        default_cube.set<data::Program>({"starShader"});
        default_cube.add<data::Mesh>();
        default_cube.set<data::Mesh>({"cube"});
        flecs::entity default_camera = world.entity("default_camera");
        default_camera.add<data::Camera>();
        default_camera.set<data::Camera>({1920, 1080, 30.0});
        default_camera.add<data::Active>();
        default_camera.add<data::Transform>();
        default_camera.get_mut<data::Transform>().position.z = 0.05;
        events::eventManager.registerEventCallback(SDL_KEYDOWN,
            [](SDL_Event *e) -> bool {
                const flecs::world world = data::ecsManager.getRegistry();
                const auto q = world.query<const data::Camera, const data::Active, const data::Transform>(); 
                flecs::entity active_camera = q.first();
                if (!active_camera.is_valid()) {
                    return false;
                }
                switch (e->key.keysym.sym) {
                case SDLK_w:    
                    active_camera.get_mut<data::Transform>().position.z -= 0.01;
                    break;
                case SDLK_s:
                    active_camera.get_mut<data::Transform>().position.z += 0.01;
                    break;
                case SDLK_a:
                    active_camera.get_mut<data::Transform>().position.x += 0.01;
                    break;
                case SDLK_d:
                    active_camera.get_mut<data::Transform>().position.x -= 0.01;
                    break;
                } 
                return true;
            }
        );
    }

    constexpr size_t TICKS_PER_SEC = 20;
    constexpr auto TIME_PER_TICK = 1000ms / TICKS_PER_SEC;

    timepoint t_last = clock::now();
    millisecond t_accumulated{0}; 

    std::atomic_bool running = true;
    events::eventManager.registerEventCallback(SDL_QUIT,
            [&](SDL_Event *e) -> bool { return (bool) (running = false); }
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
