#include <nebula/nebula.hpp>
#include <nebula/imgui.h>
#include <nebula/utils/conversion.hpp>

#include <glm/ext/quaternion_float.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <random>

void nebula::game::initialize(nebula::NebulaApi &api) {
    flecs::world &world = api.ecs->getRegistry();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0);
    for (size_t i = 0; i < 1000; i++) {
        flecs::entity cube = world.entity();
        float p_x = dis(gen), p_y = dis(gen), p_z = dis(gen);
        float s_x = dis(gen), s_y = dis(gen), s_z = dis(gen);
        cube.add<data::Transform>();
        cube.add<data::Velocity>();
        data::Transform &t = cube.get_mut<data::Transform>();
        t.position = {p_x, p_y, p_z};
        t.scale = {s_x, s_y, s_z};
        cube.add<data::Program>();
        cube.set<data::Program>({"starShader"});
        cube.add<data::Mesh>();
        cube.set<data::Mesh>({"cube"});
    }
    

    flecs::entity default_camera = world.entity("default_camera");
    default_camera.add<data::Camera>();
    default_camera.set<data::Camera>({1920, 1080, 70.0});
    default_camera.add<data::Active>();
    default_camera.add<data::Transform>();
    default_camera.get_mut<data::Transform>().position[2] = 150.0;
    api.events->registerEventCallback(nebula::events::EventType::KeyPressed,
        [&](nebula::events::Event *e) -> bool {
            flecs::world &world = api.ecs->getRegistry();
            const auto q = world.query<const data::Camera, const data::Active, const data::Transform>(); 
            flecs::entity active_camera = q.first();
            if (!active_camera.is_valid()) {
                return false;
            }
            switch ((char) e->keyboard.key_code) {
                case 'w':    
                    active_camera.get_mut<data::Transform>().position[2] -= 0.5;
                    break;
                case 's':
                    active_camera.get_mut<data::Transform>().position[2] += 0.5;
                    break;
                case 'a':
                    active_camera.get_mut<data::Transform>().position[0] += 0.5;
                    break;
                case 'd':
                    active_camera.get_mut<data::Transform>().position[0] -= 0.5;
                    break;
            } 
            return true;
        }
    );
    api.events->registerEventCallback(nebula::events::EventType::MouseMoved,
        [&](nebula::events::Event *e) {
            static float sensitivity = 0.01f;
            flecs::world &world = api.ecs->getRegistry();
            const auto q = world.query<const data::Camera, const data::Active, const data::Transform>(); 
            flecs::entity active_camera = q.first();
            if (!active_camera.is_valid()) {
                return false;
            }
            data::Transform &t = active_camera.get_mut<data::Transform>();
            glm::quat orientation;
            data::fromNebula(t.orientation, orientation);
            glm::vec3 euler = glm::eulerAngles(orientation);
            euler.x += e->mouse_motion.y_rel * sensitivity;
            euler.y += -e->mouse_motion.x_rel * sensitivity;
            orientation = glm::angleAxis(euler.x, glm::vec3{1, 0, 0});
            orientation *= glm::angleAxis(euler.y, glm::vec3{0, 1, 0});
            data::toNebula(orientation, t.orientation);
            return true;
        }
    );
    flecs::entity surface = world.entity();
    surface.add<data::Surface>();
    surface.get_mut<data::Surface>().render = [&]() -> void {
        flecs::world &world = api.ecs->getRegistry();
        const auto q = world.query<const data::Camera, const data::Active, const data::Transform>(); 
        flecs::entity active_camera = q.first();
        if (!active_camera.is_valid()) {
            return;
        }
        data::Transform &trans = active_camera.get_mut<data::Transform>();
        data::Camera &cam = active_camera.get_mut<data::Camera>();

        ImGui::Begin("Frame Rate");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Camera");
        ImGui::DragFloat3("Camera Pose", &trans.position[0]);
        ImGui::DragFloat("FOV (Degrees)", &cam.arclength);
        ImGui::End();
    };
    flecs::system gravity = world.system<data::Velocity>("Gravity")
        .each([](flecs::iter &it, size_t, data::Velocity &v) {
            v.positional[1] += -9.81f * it.delta_time(); 
        });
    flecs::system velocity = world.system<const data::Velocity, data::Transform>("Velocity")
        .each([](flecs::iter &it, size_t, const data::Velocity &v, data::Transform &t) {
            t.position[1] += 1.0f * it.delta_time(); 
        });
}
