#version 450

// outColour
layout (location = 0) out vec3 outColour;

void main() {
    // const array of poistions for triangle
    const vec3 poistions[3] = vec3[3] (
        vec3(-1.0, -1.0, 0.0),
        vec3(1.0, -1.0, 0.0),
        vec3(0.0, 1.0, 0.0)
    );

    const vec3 colour[3] = vec3[3] (
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );

    gl_Position = vec4(poistions[gl_VertexIndex], 1.0);
    outColour = colour[gl_VertexIndex];
}
