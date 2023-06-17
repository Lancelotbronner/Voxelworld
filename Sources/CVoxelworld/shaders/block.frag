#version 410 core

uniform sampler2D sampler;
uniform sampler2D sky_sampler;
uniform float timer;
uniform float daylight;
uniform int ortho;

in vec2 fragment_uv;
in float fragment_ao;
in float fragment_light;
in float fog_factor;
in float fog_height;
in float diffuse;

out vec4 result;

const float pi = 3.14159265;

void main() {
    vec3 color = vec3(texture(sampler, fragment_uv));

	// discard magenta pixels
    if (color == vec3(1.0, 0.0, 1.0))
        discard;

	// special case clouds on orthographic projection
    bool cloud = color == vec3(1.0, 1.0, 1.0);
    if (cloud && bool(ortho))
        discard;

    float df = cloud ? 1.0 - diffuse * 0.2 : diffuse;
    float ao = cloud ? 1.0 - (1.0 - fragment_ao) * 0.2 : fragment_ao;
    ao = min(1.0, ao + fragment_light);
    df = min(1.0, df + fragment_light);
    float value = min(1.0, daylight + fragment_light);
    vec3 light_color = vec3(value * 0.3 + 0.2);
    vec3 ambient = vec3(value * 0.3 + 0.2);
    vec3 light = ambient + light_color * df;
    color = clamp(color * light * ao, vec3(0.0), vec3(1.0));
    vec3 sky_color = vec3(texture(sky_sampler, vec2(timer, fog_height)));
    color = mix(color, sky_color, fog_factor);

	result = vec4(color, 1.0);
}
