#include "SDL_events.h"
#include "event.hpp"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <flecs.h>
#include <atomic>
#include <log.hpp>
#include <ratio>
#include <window.hpp>
#include <shaders.hpp>

namespace astronomy {

using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
using millisecond = std::chrono::duration<uint64_t, std::milli>;
using microsecond = std::chrono::duration<uint64_t, std::micro>;
using clock = std::chrono::high_resolution_clock;

} // namespace astronomy
  
using namespace astronomy;
using namespace astronomy::events;
using namespace std::literals::chrono_literals;

int main() {
    windows::mainWindow.initialize();
    shaders::shaderManager.initialize();

    constexpr size_t TICKS_PER_SEC = 20;
    constexpr auto TIME_PER_TICK = 1000ms / TICKS_PER_SEC;

    timepoint t_last = clock::now();
    millisecond t_accumulated{0}; 
    
    std::atomic_bool running = true;
    events::eventManager.registerEventCallback(SDL_QUIT, [&](SDL_Event *e) {
        running = false;
        return false;
    });
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
            // update systems
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        glClearColor(0.2, 0.4, 0.3, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(windows::mainWindow.window);

    }

    logger.log<INFO>("Exiting application.");
    exit(0);
}
