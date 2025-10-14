cmake_minimum_required(VERSION 3.25)
project(nebula
    VERSION 0.0.1
    DESCRIPTION "An ECS based game engine for C++ built with CMake"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

string(TIMESTAMP TODAY "%d/%m/%Y")
set(PROJECT_BUILD_DATE ${TODAY})
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/nebula/version.hpp"
    @ONLY
)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG         1.0.1
)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         release-2.32.4
)
FetchContent_Declare(
    flecs
    GIT_REPOSITORY    https://github.com/SanderMertens/flecs.git
    GIT_TAG           v4.1.1
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         v1.92.2b
)
FetchContent_GetProperties(flecs SOURCE_DIR flecs_SOURCE_DIR)
FetchContent_GetProperties(imgui SOURCE_DIR imgui_SOURCE_DIR)
FetchContent_MakeAvailable(glm SDL2 flecs imgui)

add_library(flecs_object OBJECT
    ${flecs_SOURCE_DIR}/distr/flecs.c
)
target_include_directories(flecs_object PRIVATE
    ${flecs_SOURCE_DIR}/distr
)

add_library(imgui_object OBJECT 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_object PRIVATE 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_object PUBLIC SDL2::SDL2 OpenGL::OpenGL)

add_library(nebula STATIC)
add_library(nebula::nebula ALIAS nebula)

set(SOURCES
    source/main.cpp
    source/utils/conversion.cpp
    source/utils/functions.cpp
    source/utils/log.cpp
    source/utils/math.cpp
    source/resources/meshes.cpp
    source/resources/shaders.cpp
    source/events/manager.cpp
    source/events/conversion.cpp
    source/render/manager.cpp
    source/render/types.cpp
    source/ecs/manager.cpp
    source/ecs/core.cpp
    source/ecs/components/camera.cpp
    source/ecs/components/mesh.cpp
    source/ecs/components/program.cpp
    source/ecs/components/transform.cpp
    source/ecs/components/velocity.cpp
)

set(PUBLIC_HEADERS
    include/nebula/nebula.hpp
    include/nebula/events/manager.hpp
    include/nebula/events/types.hpp
    include/nebula/ecs/ecs.hpp # meta
    include/nebula/ecs/manager.hpp
    include/nebula/ecs/components/camera.hpp
    include/nebula/ecs/components/mesh.hpp
    include/nebula/ecs/components/program.hpp
    include/nebula/ecs/components/surface.hpp
    include/nebula/ecs/components/transform.hpp
    include/nebula/ecs/components/velocity.hpp
    include/nebula/utils/log.hpp
    include/nebula/utils/math.hpp
)

set(PRIVATE_HEADERS
    include/nebula/ecs/core.hpp
    include/nebula/resources/meshes.hpp
    include/nebula/resources/shaders.hpp
    include/nebula/events/conversion.hpp
    include/nebula/utils/conversion.hpp
    include/nebula/utils/functions.hpp
)

target_sources(nebula
    PRIVATE
        ${SOURCES}
        $<TARGET_OBJECTS:flecs_object>
        $<TARGET_OBJECTS:imgui_object>
    PRIVATE
        FILE_SET private_headers 
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${PRIVATE_HEADERS}
    PRIVATE
        FILE_SET imgui_private_headers
        TYPE HEADERS
        BASE_DIRS "${imgui_SOURCE_DIR}"
        FILES
            ${imgui_SOURCE_DIR}/imgui.h
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
    PUBLIC
        FILE_SET public_headers 
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${PUBLIC_HEADERS}
)

set_target_properties(nebula PROPERTIES
    CXX_EXTENSIONS ON
    POSITION_INDEPENDENT_CODE ON
    CMAKE_CXX_VISIBILITY_PRESET hidden
    CMAKE_VISIBILITY_INLINES_HIDDEN ON
)

generate_export_header(nebula
    BASE_NAME NEBULA
    EXPORT_FILE_NAME nebula/export.hpp
)

target_include_directories(nebula
    PUBLIC
        "$<BUILD_INTERFACE:${imgui_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${flecs_SOURCE_DIR}/distr>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:include>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(nebula
    PRIVATE
        $<BUILD_INTERFACE:SDL2::SDL2>
        $<BUILD_INTERFACE:OpenGL::OpenGL>
        $<BUILD_INTERFACE:glm::glm-header-only>
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nebula-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/nebula-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nebula-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/nebula"
)

install(TARGETS nebula
    EXPORT nebula-targets
    FILE_SET public_headers DESTINATION include
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES "${flecs_SOURCE_DIR}/distr/flecs.h"
    DESTINATION include/nebula/ecs
)

install(EXPORT nebula-targets
    FILE nebula-targets.cmake
    NAMESPACE nebula::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/nebula"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nebula-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nebula-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/nebula"
)

