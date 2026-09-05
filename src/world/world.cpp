#include "world.hpp"

namespace World
{
    Chunk &World::getOrCreateChunk(int chunkX, int chunkZ)
    {
        ChunkCoord coord{chunkX, chunkZ};
        return chunks[coord];
    }

    Chunk *World::findChunk(int chunkX, int chunkZ)
    {
        ChunkCoord coord{chunkX, chunkZ};
        auto chunk = chunks.find(coord);

        return (chunk != chunks.end()) ? &chunk->second : nullptr;
    }

    BlockType World::getBlockWorld(int worldX, int worldY, int worldZ)
    {
        if (worldY < 0 || worldY >= CHUNK_SIZE)
            return BlockType::Air;

        int chunkX = worldX >= 0 ? worldX / CHUNK_SIZE : (worldX - CHUNK_SIZE + 1) / CHUNK_SIZE;
        int chunkZ = worldZ >= 0 ? worldZ / CHUNK_SIZE : (worldZ - CHUNK_SIZE + 1) / CHUNK_SIZE;

        int localX = worldX - chunkX * CHUNK_SIZE;
        int localZ = worldZ - chunkZ * CHUNK_SIZE;

        Chunk *chunk = findChunk(chunkX, chunkZ);
        if (chunk == nullptr)
            return BlockType::Air;

        return chunk->getBlock(localX, worldY, localZ).type;
    }
}
