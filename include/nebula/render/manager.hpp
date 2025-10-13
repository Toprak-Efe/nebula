#pragma once 

#include <flecs.h>
#include "../render/types.hpp"

namespace nebula::rendering {

class RenderingManager {
public:
    ~RenderingManager();
    RenderingManager(const RenderingManager &) = delete;
    RenderingManager(RenderingManager &&) = delete;
    RenderingManager& operator=(const RenderingManager&) = delete;
    static RenderingManager &get();
    void initialize();
    void drawAll() const;
private:
    RenderingManager();
    windows::SDLWindow m_mainWindow;
    void renderObjects(const flecs::entity &camera) const;
    void renderSurfaces(flecs::entity &camera) const;
}; // class RenderingManager

}; // namespace nebula::renderng

