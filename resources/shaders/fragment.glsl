#version 330 core
out vec4 FragColor;
uniform float time;

void main() {
    //time
    FragColor = vec4((sin(time)+1)/2, (sin(2*time)+1)/2, (sin(3*time)+1)/2, 1.0);
}