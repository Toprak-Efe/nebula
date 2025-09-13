cmake_minimum_required(VERSION 4.0)
project(astronomy)

include(GNUInstallDirs)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -g -O1 -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_ASAN "-fsanitize=address -g -O1 -fno-omit-frame-pointer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING 
        "Choose the build type: Debug, Release, or ASan")
endif()

set(SOURCES
    source/main.cpp
    source/event.cpp
    source/functions.c
    source/window.cpp
    source/shaders.cpp
)

include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG         1.0.1	
)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.4
    GIT_PROGRESS TRUE
)
FetchContent_Declare(
  flecs
  GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
  GIT_TAG v4.1.1
)
FetchContent_Declare(
    imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.92.2b 
)
FetchContent_MakeAvailable(glm SDL2 flecs imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui
    SDL2::SDL2
    OpenGL::OpenGL
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)
install(TARGETS ${CMAKE_PROJECT_NAME})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_dependencies(${PROJECT_NAME}
    imgui
    OpenGL::OpenGL
    SDL2::SDL2
    glm::glm
    flecs::flecs_static
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    OpenGL::OpenGL
    SDL2::SDL2
    glm::glm
    flecs::flecs_static
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

