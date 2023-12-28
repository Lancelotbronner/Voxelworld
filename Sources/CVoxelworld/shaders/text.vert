#version 410 core

uniform mat4 matrix;

// how they seem to be setup
//layout(location=1) in vec4 position;
//layout(location=0) in vec2 uv;

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;

out vec2 fragment_uv;

void main() {
    gl_Position = matrix * position;
    fragment_uv = uv;
}
