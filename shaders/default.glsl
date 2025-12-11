#shader vertex
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 transformation;

out vec3 vertices;

void main() {
    gl_Position = transformation * vec4(position, 1.0);
    vertices = position;
}

#shader fragment
#version 460 core

in vec3 gs_vertices;
out vec4 frag_color;

uniform vec3 color;

void main() {
    frag_color = vec4(color, 1.0);
}
