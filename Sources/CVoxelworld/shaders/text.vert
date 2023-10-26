#version 410 core

uniform vec2 window;

in vec2 position;
in vec2 uv;

out vec2 fragment_uv;

void main() {
	vec2 positionHS = position / window;
	gl_Position = vec4(positionHS, 0, 1);
    fragment_uv = uv;
}
