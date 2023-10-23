#version 410 core

uniform sampler2D sampler;
uniform bool is_sign;

in vec2 fragment_uv;
out vec4 color;

void main() {
	color = vec4(1,0,0,1);
//    color = texture(sampler, fragment_uv);

	if (is_sign && color == vec4(1.0))
		discard;

	color.a = max(color.a, 0.4);
}
