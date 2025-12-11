cmake_minimum_required(VERSION 3.31)
project(astronomy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Nebula REQUIRED)

add_executable(${PROJECT_NAME}
    source/game.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Nebula::Nebula
    SDL2::SDL2
    OpenGL::OpenGL
)

