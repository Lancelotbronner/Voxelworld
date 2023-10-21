# Roadmap

## Tile System

Tiles can specify their geometry builder, a simple function taking a `geometry_t`.

## Chunks

Chunks will be 32x32x32 so their index can be represented with 32-bit integers.

Chunks will generate (and regenerate) their meshes by iterating their tiles and calling
the tile's geometry builder.

## Geometry

Terrain geometry will all use the same vertex format:

```
// The tile index of the vertex
uint tile;

// The UVs of the tile
float[2] uv;

// The result of lighting
float[3] tint;
```

From this information, the UVs can be calculated easily:

```
// The size of a single tile in the texture
uniform float UNIT;

uint _u = texture + (uv >> 1)
float u = _u * UNIT;

uint _v = texture + (uv & 1)
float v = _v * UNIT;
```
