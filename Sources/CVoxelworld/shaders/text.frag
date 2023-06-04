#version 410 core

uniform sampler2D sampler;
uniform bool is_sign;

out vec4 color;
out vec2 fragment_uv;

void main() {
    color = texture(sampler, fragment_uv);
    if (is_sign) {
        if (color == vec4(1.0)) {
            discard;
        }
    }
    else {
        color.a = max(color.a, 0.4);
    }
}
