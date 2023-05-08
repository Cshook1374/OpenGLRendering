#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec3 vertexColor;
out vec2 pass_textureCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main(void) {
    gl_Position = viewMatrix * modelMatrix * vec4(position, 1.0);
    vertexColor = vec3(position.x + 0.5, position.y + 0.5, position.z + 0.5);
    pass_textureCoords = textureCoords;
}