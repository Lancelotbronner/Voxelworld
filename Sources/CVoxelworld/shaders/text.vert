#version 410 core

uniform mat4 projection;

in vec2 position;
in vec2 uv;

out vec2 fragment_uv;

void main() {
	gl_Position = projection * vec4(position, 0, 0);
    fragment_uv = uv;
}
