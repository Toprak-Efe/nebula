#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <glm/glm.hpp>
#include <flecs.h>
#include <ecs/ecsmanager.hpp>
#include <ecs/components/camera.hpp>
#include <ecs/components/mesh.hpp>
#include <ecs/components/program.hpp>
#include <ecs/components/transform.hpp>
#include <meshes.hpp>
#include <window.hpp>

namespace astronomy::rendering {

class RenderingManager {
public:
    void initialize();
    void drawAll() const;
private:
    flecs::system m_object_render_system;
    flecs::system m_surface_render_system;
    void renderObjects(const glm::mat4 &mat_pv) const;
    void renderSurfaces() const;
}; // class RenderingManager

inline RenderingManager manager;

}; // namespace astronomy::renderng

#endif//RENDERING_HPP
