#version 460 core

layout (points) in;
layout (points, max_vertices = 32) out;

in vec3 vertices[];
out vec3 gs_vertices; 

void main() {
    gs_vertices = vertices[0];
    
    for (int i = -5; i <= 5; i++) {
        gl_Position = gl_in[0].gl_Position + i*vec4(0.005, 0.0, 0.0, 0.0);
        EmitVertex();
    }
    for (int i = -5; i <= 5; i++) {
        gl_Position = gl_in[0].gl_Position + i*vec4(0.0, 0.005, 0.0, 0.0);
        EmitVertex();
    }

    EndPrimitive();
}
