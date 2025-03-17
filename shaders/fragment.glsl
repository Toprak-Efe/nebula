#version 460 core

in vec3 gs_vertices;
out vec4 frag_color;

void main() {
    vec3 color = normalize(gs_vertices);
    frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}