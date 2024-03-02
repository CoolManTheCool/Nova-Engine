#version 450

vec2 positions[3];

void main() {
    positions[0] = vec2(0.0, -0.5);
    positions[1] = vec2(0.5, 0.5);
    positions[2] = vec2(-0.5, 0.5);

    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
