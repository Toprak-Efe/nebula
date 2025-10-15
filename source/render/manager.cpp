#include <imgui.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <flecs.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "../../include/nebula/ecs/ecs.hpp"
#include "../../include/nebula/utils/functions.hpp"
#include "../../include/nebula/resources/shaders.hpp"
#include "../../include/nebula/resources/meshes.hpp"
#include "../../include/nebula/ecs/components/camera.hpp"
#include "../../include/nebula/utils/math.hpp"
#include "../../include/nebula/utils/conversion.hpp"
#include "../../include/nebula/render/manager.hpp"

using namespace nebula::windows;
using namespace nebula::rendering;

RenderingManager::RenderingManager() {
    return;
}

RenderingManager::~RenderingManager() {
    return;
}


void RenderingManager::initialize() {
    flecs::world world = nebula::data::ECSManager::get().getRegistry();
    flecs::entity main_camera = world.entity("main_camera");
    main_camera.add<data::Camera>();
    main_camera.add<data::Active>();
    m_mainWindow.initialize();
}

void RenderingManager::drawAll() const {
    /* Fetch Camera */
    flecs::world world = nebula::data::ECSManager::get().getRegistry();
    const auto q = world.query<const data::Camera, const data::Active, const data::Transform>(); 
    flecs::entity active_camera = q.first();
    if (!active_camera.is_valid()) {
        return;
    }

    glClearColor(0.2, 0.4, 0.3, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    renderObjects(active_camera);
    renderSurfaces(active_camera);

    SDL_GL_SwapWindow(m_mainWindow.window);
}

void RenderingManager::renderObjects(const flecs::entity &camera) const {
    resources::MeshManager &meshManager = resources::MeshManager::get();
    resources::ShaderManager &shaderManager = resources::ShaderManager::get();

    data::Mat4 proj = camera.get<data::Camera>().getProjectionMatrix();
    glm::mat4 proj_glm;
    data::fromNebula(proj, proj_glm);
    data::Mat4 inv = camera.get<data::Transform>().getInverseModelTransform();
    glm::mat4 inv_glm;
    data::fromNebula(inv, inv_glm);

    glm::mat4 mat_pv = proj_glm * inv_glm; 
    flecs::world world = nebula::data::ECSManager::get().getRegistry();
    const auto q = world.query<const data::Mesh, const data::Program, const data::Transform>();
    q.each([&](flecs::entity e, const data::Mesh &m, const data::Program &p, const data::Transform &t) {

        const resources::Mesh &msh = meshManager.getMesh(m.name);
        const resources::ShaderProgram &prg = shaderManager.getProgram(p.name);
        GLint u_trans = prg.getUniform("transformation");
        GLint u_color = prg.getUniform("color");
        data::Mat4 mt = t.getModelTransform();
        glm::mat4 mt_glm;
        data::fromNebula(mt, mt_glm);
        glm::mat4 mvp = mat_pv * mt_glm;
        glUniformMatrix4fv(u_trans, 1, GL_FALSE, glm::value_ptr(mvp)); 
        glUniform3f(u_color, 0.1, 0.1, 0.1);
        glUseProgram(prg.getId());
        glBindVertexArray(msh.vao);
        glDrawArrays(GL_TRIANGLES, 0, msh.vertex_count/3);
    });
}

void RenderingManager::renderSurfaces(flecs::entity &camera) const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    /*data::Transform &trans = camera.get_mut<data::Transform>();
    data::Camera &cam = camera.get_mut<data::Camera>();

    ImGui::Begin("Frame Rate");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Begin("Camera");
    ImGui::DragFloat3("Camera Pose", &trans.position[0]);
    ImGui::DragFloat("FOV (Degrees)", &cam.arclength);
    ImGui::End();
    */

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

RenderingManager &RenderingManager::get() {
    static RenderingManager renderingManager;
    return renderingManager;
}
