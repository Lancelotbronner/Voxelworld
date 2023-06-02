#include <noise.h>

/*
 * Permutation table. This is just a random jumble of all numbers 0-255,
 * repeated twice to avoid wrapping the index at 255 for each lookup.
 * This needs to be exactly the same for all instances on all platforms,
 * so it's easiest to just keep it as static explicit data.
 * This also removes the need for any initialisation of this class.
 *
 * Note that making this an int[] instead of a char[] might make the
 * code run faster on platforms with a high penalty for unaligned single
 * byte addressing. Intel x86 is generally single-byte-friendly, but
 * some other CPUs are faster with 4-aligned reads.
 * However, a char[] is smaller, which avoids cache trashing, and that
 * is probably the most important aspect on most architectures.
 * This array is accessed a *lot* by the noise functions.
 * A vector-valued noise over 3D accesses it 96 times, and a
 * float-valued 4D noise 64 times. We want this to fit in the cache!
 */
unsigned char perm[512];

/*
 * Helper functions to compute gradients-dot-residualvectors (1D to 4D)
 * Note that these generate gradients of more than unit length. To make
 * a close match with the value range of classic Perlin noise, the final
 * noise values need to be rescaled to fit nicely within [-1,1].
 *
 * To match the RenderMan noise in a statistical sense, the approximate
 * scaling values (empirically determined from test renderings) are:
 * 1D noise needs rescaling with 0.188
 * 2D noise needs rescaling with 0.507
 * 3D noise needs rescaling with 0.936
 * 4D noise needs rescaling with 0.87
 *
 * (The simplex noise functions as such also have different scaling.)
 * Note also that these noise functions are the most practical and useful
 * signed version of Perlin noise. To return values according to the
 * RenderMan specification from the SL noise() and pnoise() functions,
 * the noise values need to be scaled and offset to [0,1], like this:
 * float SLnoise = (noise(x,y,z) + 1.0) * 0.5;
 */

static inline float grad1( int hash, float x ) {
	int h = hash & 15;
	float grad = 1.0f + (h & 7);   // Gradient value 1.0, 2.0, ..., 8.0
	if (h&8) grad = -grad;         // Set a random sign for the gradient
	return ( grad * x );           // Multiply the gradient with the distance
}

static inline float grad2( int hash, float x, float y ) {
	int h = hash & 7;      // Convert low 3 bits of hash code
	float u = h<4 ? x : y;  // into 8 simple gradient directions,
	float v = h<4 ? y : x;  // and compute the dot product with (x,y).
	return ((h&1)? -u : u) + ((h&2)? -2.0f*v : 2.0f*v);
}

static inline float grad3( int hash, float x, float y , float z ) {
	int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
	float u = h<8 ? x : y; // gradient directions, and compute dot product.
	float v = h<4 ? y : h==12||h==14 ? x : z; // Fix repeats at h = 12 to 15
	return ((h&1)? -u : u) + ((h&2)? -v : v);
}

static inline float grad4( int hash, float x, float y, float z, float t ) {
	int h = hash & 31;      // Convert low 5 bits of hash code into 32 simple
	float u = h<24 ? x : y; // gradient directions, and compute dot product.
	float v = h<16 ? y : z;
	float w = h<8 ? z : t;
	return ((h&1)? -u : u) + ((h&2)? -v : v) + ((h&4)? -w : w);
}
