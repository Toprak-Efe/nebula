#version 460 core

in vec3 gs_vertices;
out vec4 frag_color;

uniform vec3 color;

void main() {
    frag_color = vec4(color, 1.0);
}