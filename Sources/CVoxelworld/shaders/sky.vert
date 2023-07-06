#version 410 core

uniform mat4 matrix;

in vec4 position;
in vec3 normal;
in vec2 uv;

out vec2 fragment_uv;

void main() {
    gl_Position = matrix * position;
    fragment_uv = uv;
}