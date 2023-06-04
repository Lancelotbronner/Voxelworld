#version 410 core

uniform sampler2D sampler;
uniform float timer;

out vec4 color;
out vec2 fragment_uv;

void main() {
    vec2 uv = vec2(timer, fragment_uv.t);
    color = texture(sampler, uv);
}
