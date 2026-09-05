#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>
#include <vector>

#include "raylib.h"
#include "block.hpp"

namespace World
{
    class World; // including world.hpp here makes some define problems

    constexpr int CHUNK_SIZE = 4;
    constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    struct ChunkCoord
    {
        int x, z; // y later

        bool operator==(const ChunkCoord &other) const
        {
            return x == other.x && z == other.z;
        }
    };

    struct ChunkCoordHash
    {
        size_t operator()(const ChunkCoord &coord) const
        {
            return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 1);
        }
    };

    class Chunk
    {
    private:
        static int index(int x, int y, int z);
        bool isAirWorld(World &world, int chunkX, int chunkZ, int x, int y, int z) const;
        
        std::array<Block, CHUNK_VOLUME> blocks;

    public:
        Chunk();

        Block getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, BlockType type);

        Mesh buildMesh(World &world, int chunkX, int chunkZ) const;
    };
}

#endif