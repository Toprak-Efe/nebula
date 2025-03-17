cmake_minimum_required(VERSION 3.0)
project(astronomy)

## Start Defining Variables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DDEBUG -DCAMERA_MOVEMENT_CONTROLS")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES
    source/main.c
    source/log.c
    source/coordinates.c
    source/functions.c
    source/camera.c
    source/window.c
    source/shader.c
    source/scene.c
)
## End Defining Variables

##  Begin Embedding Shaders
message(STATUS "Embedding shaders.")
find_program(OBJCOPY_EXECUTABLE objcopy)
if(NOT OBJCOPY_EXECUTABLE)
    message(FATAL_ERROR "objcopy not found.")
endif()
file(GLOB SHADER_FILES "shaders/*.glsl")
set(OBJECT_FILES)
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
    string(REPLACE "/" "_" SECTOR ${SHADER_FILE})
    string(REPLACE "." "_" SECTOR ${SECTOR})
    set(SECTOR_START _binary_${SECTOR}_start)
    set(SECTOR_END _binary_${SECTOR}_end)
    set(SECTOR_SIZE _binary_${SECTOR}_size)
    set(SECTOR_START_NEW _binary__${SHADER_NAME}_start)
    set(SECTOR_END_NEW _binary__${SHADER_NAME}_end)
    set(SECTOR_SIZE_NEW _binary__${SHADER_NAME}_size)
    set(OBJECT_FILE "${SHADER_NAME}.o")
    set(OBJECT_FILE_RAW "${SHADER_NAME}.raw.o")
    add_custom_command(
        OUTPUT ${OBJECT_FILE_RAW}
        COMMAND ${OBJCOPY_EXECUTABLE}
        ARGS -I binary -O elf64-x86-64 -B i386 ${SHADER_FILE} ${OBJECT_FILE_RAW}
        DEPENDS ${SHADER_FILE}
    )
    add_custom_command(
        OUTPUT ${OBJECT_FILE}
        COMMAND ${OBJCOPY_EXECUTABLE}
        ARGS --redefine-sym ${SECTOR_START}=${SECTOR_START_NEW} --redefine-sym ${SECTOR_END}=${SECTOR_END_NEW} --redefine-sym ${SECTOR_SIZE}=${SECTOR_SIZE_NEW} ${OBJECT_FILE_RAW} ${OBJECT_FILE}
        DEPENDS ${OBJECT_FILE_RAW}
    )
    list(APPEND OBJECT_FILES ${OBJECT_FILE})
endforeach()
## End Embedding Shaders

## Start Fetching Libraries
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
fetchcontent_declare(
  cglm
  GIT_REPOSITORY https://github.com/recp/cglm.git
  GIT_TAG v0.9.6
  GIT_PROGRESS TRUE
)
fetchcontent_makeavailable(cglm)
fetchcontent_declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.0
    GIT_PROGRESS TRUE
)
fetchcontent_makeavailable(SDL2)
## End Fetching Libraries

## Start Building Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${OBJECT_FILES})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_dependencies(${PROJECT_NAME}
    OpenGL::OpenGL
    SDL2::SDL2
    cglm
)

target_link_libraries(${PROJECT_NAME}
    OpenGL::OpenGL
    SDL2::SDL2
    cglm
    m
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
## End Building Executable
