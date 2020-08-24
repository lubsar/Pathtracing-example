#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 TextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TextureCoords = textureCoords;
    gl_Position = projection * view * model * vec4(position, 1.0f);
};
