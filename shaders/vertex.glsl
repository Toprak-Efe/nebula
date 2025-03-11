#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 transformation;

out vec3 vertex_position;

void main() {
    gl_Position = transformation * vec4(position, 1.0);
    vertex_position = position;
}