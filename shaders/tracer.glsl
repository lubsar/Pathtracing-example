#version 430 core

#define MAX_SCENE_BOUNDARY 1000.0f
#define M_PI 3.1415f

//=== inputs ===
layout (local_size_x = 8, local_size_y = 8) in;
layout(std430, binding = 4) buffer sphere_buffer
{
    vec4 spheres[];
};

// camera
uniform vec3 eye;
uniform vec3 top_left;
uniform vec3 top_right;
uniform vec3 bottom_left;
uniform vec3 bottom_right;

uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_intensity;

//=== outputs ===
layout(std430, binding = 5) buffer debug_buffer
{
    // should not use vec 3, because of memory alignment?
    vec4 debug_data[];
};
layout(binding = 1, rgba32f) uniform image2D outputImage;
//=======

struct Sphere {
    vec3 center;
    float radius;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

vec3 hitPoint(const Ray ray, const float t) {
    return ray.origin + ray.direction * t;
}

bool intersectsSphere(const Ray ray, const Sphere sp, out float t) {
    float radiusSq = sp.radius * sp.radius;
    vec3 dist = sp.center - ray.origin;
    float tca = dot(dist, ray.direction);

    //behind origin
    if (tca < 0) {
        return false;
    }

    float d2 = dot(dist, dist) - tca * tca;

    if (d2 > radiusSq) {
        return false;
    }

    float thc = sqrt(radiusSq - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    t0 = min(t0, t1);
    t1 = max(t0, t1);

    if(t0 < 0.0f) {
        t0 = t1;
        if(t0 < 0.0f) {
            return false;
        }
    }

    t = t0;
    return true;
}

float illuminate(vec3 point, vec3 normal, ivec2 pixel) {
    vec3 distance = light_position - point;
    vec3 direction = normalize(distance);

    float closest_t = MAX_SCENE_BOUNDARY;
    float hit = 0.0f;
    int sphere_index = -1;

    for (int i = 0; i < spheres.length(); i += 2){
        vec4 data = spheres[i];
        if(intersectsSphere(Ray(point, direction), Sphere(data.xyz, data.w), hit)) {
            if(closest_t > hit) {
                closest_t = hit;
                sphere_index = i;
            }
        }
    }

    // shadow ray: primary ray hitpoint -> light
    debug_data[(pixel.x + pixel.y * imageSize(outputImage).x)* 2 + 1] = vec4(light_position, 1.0);

    if(sphere_index > -1) {
        return 0.0f;
    } else {
        return max(max((light_intensity - length(distance)), 0.0f) * dot(direction, normal) / light_intensity, 0.05f);
    }
}

vec4 trace(const Ray ray, ivec2 pixel) {
    float closest_t = MAX_SCENE_BOUNDARY;
    float hit = 0.0f;
    int sphere_index = -1;

    for (int i = 0; i < spheres.length(); i += 2){
        vec4 data = spheres[i];
        if(intersectsSphere(ray, Sphere(data.xyz, data.w), hit)) {
            if(closest_t > hit) {
                closest_t = hit;
                sphere_index = i;
            }
        }
    }

    vec3 hitpoint = hitPoint(ray, closest_t);

    // primary ray: origin(eye) -> hitpoint
    debug_data[(pixel.x + pixel.y * imageSize(outputImage).x)*2] = vec4(hitpoint, 1.0);

    if(sphere_index > -1) {
        return spheres[sphere_index + 1] * illuminate(hitpoint, normalize(hitpoint - spheres[sphere_index].xyz), pixel);
    } else {
        debug_data[(pixel.x + pixel.y * imageSize(outputImage).x)*2+1] = vec4(light_position, 0.0);
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(outputImage);

    if (pixel.x >= size.x || pixel.y >= size.y) {
        return;
    }

    vec3 hor_step = (top_right - top_left) / size.x;
    vec3 vert_step = (top_left - bottom_left) / size.y;
    vec3 pixel_center = top_left + hor_step / 2.0f + hor_step * pixel.x - vert_step / 2.0f - vert_step * pixel.y;
    vec3 direction = normalize(pixel_center - eye);

    vec4 color = trace(Ray(eye, direction), pixel);
    imageStore(outputImage, pixel, color);
}
