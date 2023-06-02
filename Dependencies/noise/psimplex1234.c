#include "_noise.h"

#define simplex(octaves, persistence, lacunarity, step) { \
	float frequency = 1.0f; \
	float amplitude = 1.0f; \
	float max = 1.0f; \
	float total = step; \
	for (int i = 1; i < octaves; i++) { \
		frequency *= lacunarity; \
		amplitude *= persistence; \
		max += amplitude; \
		total += step * amplitude; \
	} \
	return (1 + total / max) / 2; \
}

float simplex1( float x, int octaves, float persistence, float lacunarity ) {
	simplex(octaves, persistence, lacunarity, snoise1(x * frequency))
}

float simplex2( float x, float y, int octaves, float persistence, float lacunarity ) {
	simplex(octaves, persistence, lacunarity, snoise2(x * frequency, y * frequency))
}

float simplex3( float x, float y, float z, int octaves, float persistence, float lacunarity ) {
	simplex(octaves, persistence, lacunarity, snoise3(x * frequency, y * frequency, z * frequency))
}

float simplex4( float x, float y, float z, float w, int octaves, float persistence, float lacunarity ) {
	simplex(octaves, persistence, lacunarity, snoise4(x * frequency, y * frequency, z * frequency, w * frequency))
}
