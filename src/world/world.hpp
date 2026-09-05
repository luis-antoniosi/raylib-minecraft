#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>

#include "chunk.hpp"

namespace World
{
    class World
    {
    private:
        std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> chunks;

    public:
        Chunk &getOrCreateChunk(int chunkX, int chunkZ);
        Chunk *findChunk(int chunkX, int chunkZ);
        BlockType getBlockWorld(int worldX, int worldY, int worldZ);
    };
}

#endif