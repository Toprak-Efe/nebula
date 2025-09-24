#include "ecs/components/mesh.hpp"
#include "ecs/components/transform.hpp"
#include "functions.h"
#include "glm/gtc/type_ptr.hpp"
#include "meshes.hpp"
#include "shaders.hpp"
#include <GL/gl.h>
#include <rendering.hpp>

using namespace astronomy::windows;
using namespace astronomy::rendering;

void RenderingManager::initialize() {
    flecs::world world = data::ecsManager.getRegistry();
    flecs::entity main_camera = world.entity("main_camera");
    main_camera.add<data::Camera>();
    main_camera.add<data::Active>();
    mainWindow.initialize();
}

void RenderingManager::drawAll() const {
    /* Fetch Camera */
    const flecs::world world = data::ecsManager.getRegistry();
    const auto q = world.query<const data::Camera, const data::Active, const data::Transform>(); 
    flecs::entity active_camera = q.first();
    if (!active_camera.is_valid()) {
        return;
    }
    glm::mat4 mat_pv = active_camera.get<data::Camera>().getProjectionMatrix() * active_camera.get<data::Transform>().getInverseModelTransform();

    glClearColor(0.2, 0.4, 0.3, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    renderObjects(mat_pv);
    renderSurfaces();

    SDL_GL_SwapWindow(mainWindow.window);
}

void RenderingManager::renderObjects(const glm::mat4 &mat_pv) const {
    const flecs::world world = data::ecsManager.getRegistry();
    const auto q = world.query<const data::Mesh, const data::Program, const data::Transform>();
    q.each([&](flecs::entity e, const data::Mesh &m, const data::Program &p, const data::Transform &t) {
        const resources::Mesh &msh = resources::meshManager.getMesh(m.name);
        const resources::ShaderProgram &prg = resources::shaderManager.getProgram(p.name);
        GLint u_trans = prg.getUniform("transformation");
        GLint u_color = prg.getUniform("color");
        glm::mat4 mvp = mat_pv * t.getModelTransform();

        glUniformMatrix4fv(u_trans, 1, GL_FALSE, glm::value_ptr(mvp)); 
        glUniform3f(u_color, 0.5, 0.5, 0.5);
    
        glUseProgram(prg.getId());
        glBindVertexArray(msh.vao);
        
        glDrawArrays(GL_TRIANGLES, 0, msh.vertex_count);
    });
}

void RenderingManager::renderSurfaces() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Astronomy");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

