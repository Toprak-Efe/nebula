#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 200) out;

void main() {
    int numSegments = 32;
    float radius = 0.1;

    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0 * 3.14159265358979323846 * float(i) / float(numSegments);
        vec3 offset = vec3(radius * cos(angle), radius * sin(angle), 0.0);
        gl_Position = gl_in[0].gl_Position + vec4(offset, 0.0);
        EmitVertex();
    }
    EndPrimitive();
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0 * 3.14159265358979323846 * float(i) / float(numSegments);
        vec3 offset = vec3(0.0, radius * cos(angle), radius * sin(angle));
        gl_Position = gl_in[0].gl_Position + vec4(offset, 0.0);
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0 * 3.14159265358979323846 * float(i) / float(numSegments);
        vec3 offset = vec3(radius * cos(angle),0.0, radius * sin(angle));
        gl_Position = gl_in[0].gl_Position + vec4(offset, 0.0);
        EmitVertex();
    }
    EndPrimitive();
}