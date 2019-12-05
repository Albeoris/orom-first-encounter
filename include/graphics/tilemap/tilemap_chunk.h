#ifndef TILEMAP_CHUNK_H
#define TILEMAP_CHUNK_H

//8x1 chunk
struct TileMapChunk {
    uint16_t min_y;
    uint16_t max_y;
    uint8_t start_tile_i;
    uint8_t tile_j;
    uint16_t tile_id[8];
    uint8_t top_heights[16];
    uint8_t bottom_heights[16];
}

#endif /* end of include guard: TILEMAP_CHUNK_H */
