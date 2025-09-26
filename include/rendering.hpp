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

namespace nebula::rendering {

class RenderingManager {
public:
    void initialize();
    void drawAll() const;
private:
    void renderObjects(const flecs::entity &camera) const;
    void renderSurfaces(flecs::entity &camera) const;
}; // class RenderingManager

inline RenderingManager manager;

}; // namespace nebula::renderng

#endif//RENDERING_HPP
