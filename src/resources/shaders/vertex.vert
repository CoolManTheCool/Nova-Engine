#version 450
layout(location = 0) in vec2 position;

layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset;
	float time;
} push;

void main() {
    vec2 VertexPosition = push.transform * position + push.offset;

    gl_Position = vec4(VertexPosition, 0.0, 1.0);
}