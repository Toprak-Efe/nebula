# 🌌 Nebula Engine

**Nebula** is a game engine for **C++**, which is based around ECS with a rudimentary event support. It is designed to be used through **CMake** and is installed as a static library .

---

## Features

- **GLM:** Provides conversions from engine-compatible types to **GLM** types. 
- **ECS:** Built on **flecs** library for fast, data-oriented design.  
- **SDL2:** Uses **SDL2** for windowing, input, and platform abstraction.  
- **OpenGL:** Integrated with **OpenGL** for rendering. Includes helpers for managing shaders and meshes.  
- **C++23:** Built using the latest C++ standard, with a lot of suboptimal code. 
- **UI Tools:** Comes with **ImGui** for easy creation of debug UIs and in-game tools.  
- **CMake Integration:** A **CMake** build system that uses **FetchContent** for dependency management and usability with`find_package`.  

---

## Dependencies

Nebula pulls in its dependencies automatically using CMake's **FetchContent**. To compile it with your game, you need to install SDL2 and OpenGL, however. 

- **ImGui:** For the debug GUI.  
- **GLM:** For mathematics.  
- **flecs:** The core Entity Component System.  
- **SDL2:** For windowing, input, and platform abstraction.  
- **OpenGL:** For rendering (must be available on the system).  

---

## Building Nebula

You will need a **C++23 compatible compiler** and **CMake** installed.

```bash
# 1. Clone the repository
git clone https://github.com/Toprak-Efe/nebula.git 
cd nebula

# 2. Configure the project
# This will download all dependencies automatically
cmake -S . -B build

# 3. Build the project
cmake --build build --parallel

# 4. (Optional) Install the library
# This will place the library and headers in a system-friendly location
cmake --build build --target install

---

## Making The Game

You need only define the function `void nebula::game::initialize(nebula::NebulaApi &api)` and link it against libnebula along with SDL2 and OpenGL. You may find an example project within the `example` folder.

