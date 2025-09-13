#ifndef SCENE_HPP
#define SCENE_HPP

namespace astronomy::scene {

class Scene {
public:
    Scene();
    Scene(Scene &&) = default;
    Scene(const Scene &) = default;
    Scene &operator=(Scene &&) = default;
    Scene &operator=(const Scene &) = default;
    ~Scene();
    void draw();
private:
};

inline Scene mainScene;

}

#endif//SCENE_HPP

