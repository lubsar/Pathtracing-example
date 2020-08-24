#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 center;

out vec3 Color;

uniform mat4 view;
uniform mat4 projection;

void main() {
    Color = color.rgb;
    mat4 model = mat4(1.0f);
    model[0][0] = center.w;
    model[1][1] = center.w;
    model[2][2] = center.w;

    model[3] = vec4(center.xyz, 1.0f);

    gl_Position =  projection * view * model* vec4(position, 1.0f);
};
