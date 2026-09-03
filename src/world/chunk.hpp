#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>

#include "block.hpp"

namespace World
{
    constexpr int CHUNK_SIZE = 8;
    constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    class Chunk
    {
    private:
        static int index(int x, int y, int z)
        {
            return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
        }

        std::array<BlockType, CHUNK_VOLUME> blocks;

    public:
        Chunk();

        BlockType getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, BlockType type);
    };
}

#endif