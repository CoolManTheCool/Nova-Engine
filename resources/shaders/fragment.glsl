#version 330 core

out vec4 FragColor;

uniform vec2 resolution;
uniform float time;

const int MAX_STEPS = 100;
const float EPSILON = 0.01;

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Hit {
    float distance;
    int objectID;
};

// Function to compute distance to the nearest object
float distance(vec3 point) {
    // Distance to sphere
    vec3 sphereCenter = vec3(0.0, 0.0, -3.0);
    float sphereRadius = 1.0;
    float distSphere = length(point - sphereCenter) - sphereRadius;

    // Distance to plane
    float distPlane = point.y;

    return min(distSphere, distPlane);
}

// Function to perform ray marching
Hit rayMarch(Ray ray) {
    Hit hit;
    hit.distance = 0.0;
    hit.objectID = -1;

    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 point = ray.origin + ray.direction * hit.distance;
        float dist = distance(point);

        if (dist < EPSILON) {
            hit.distance = length(point - ray.origin);
            break;
        }

        hit.distance += dist;
        if (hit.distance > 100.0) {
            hit.distance = -1.0; // No intersection, set distance to -1
            break;
        }
    }

    return hit;
}

// Main function
void main() {
    vec2 uv = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    Ray ray;
    ray.origin = vec3(0.0, 0.0, 0.0);
    ray.direction = normalize(vec3(uv, -1.0));

    Hit hit = rayMarch(ray);

    if (hit.distance > 0.0) {
        // Intersection, color the pixel
        vec3 color;
        if (hit.objectID == 0) {
            color = vec3(0.5, 0.5, 0.8); // Sphere color
        } else if (hit.objectID == 1) {
            color = vec3(0.8, 0.8, 0.8); // Plane color
        }
        FragColor = vec4(color, 1.0);
    } else {
        // No intersection, set background color
        FragColor = vec4(0.2, 0.2, 0.2, 1.0);
    }
}
