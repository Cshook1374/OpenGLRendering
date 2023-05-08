#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec3 vertexColor;

void main(void) {
    gl_Position = vec4(position, 1.0);
    vertexColor = vec3(position, 1.0);
}