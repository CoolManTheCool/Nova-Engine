#version 330 core

layout(location = 0) in vec3 aPosition; // Attribute for vertex position

void main() {
    gl_Position = vec4(aPosition, 1.0); // Set the position of the vertex
}
