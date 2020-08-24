#version 430 core

in vec2 TextureCoords;
out vec4 outColor;

layout(binding = 1, rgba32f) uniform sampler2D tex;

void main() {
    outColor = texture(tex, TextureCoords);
};
