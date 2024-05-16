#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projectionViewMatrix;
  vec3 directionToLight;
} UBO;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

const float AMBIENT = 0.02;
const vec3 LIGHT_COLOR = vec3(0.0, 0.9, 0.2);
void main() {
  gl_Position = UBO.projectionViewMatrix * push.modelMatrix * vec4(position, 1.0);

  vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

  float lightIntensity = max(dot(normalWorldSpace, UBO.directionToLight), 0);

  fragColor = AMBIENT + (lightIntensity * LIGHT_COLOR) * color;
}