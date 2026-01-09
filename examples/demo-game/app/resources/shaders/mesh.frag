#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;
layout (location = 3) in vec2 fragUV;

layout (location = 0) out vec4 outColor;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 invView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[100];
  int numLights;
} ubo;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
  float roughness;
} push;

void main() {
  vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
  vec3 specularLight = vec3(0.0);
  vec3 surfaceNormal = normalize(fragNormalWorld);

  vec3 cameraPosWorld = ubo.invView[3].xyz;
  vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

  for (int i = 0; i < ubo.numLights; i++) {
    PointLight light = ubo.pointLights[i];
    vec3 directionToLight = light.position.xyz - fragPosWorld;
    float distanceSquared = dot(directionToLight, directionToLight);
    float maxDistance = light.color.w * 15.0; 
    directionToLight = normalize(directionToLight);

    if (distanceSquared < maxDistance * maxDistance) {
      float attenuation = 1.0 / (distanceSquared + 1.0);
      float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0.0);
      vec3 intensity = light.color.xyz * light.color.w * attenuation;

      diffuseLight += intensity * cosAngIncidence;

      vec3 halfAngle = normalize(directionToLight + viewDirection);

      float specPower = mix(128.0, 1.0, push.roughness);

      float blinnTerm = pow(clamp(dot(surfaceNormal, halfAngle), 0, 1), specPower);

      specularLight += intensity * blinnTerm * 0.5; 
    }
  }
  
  outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
}
