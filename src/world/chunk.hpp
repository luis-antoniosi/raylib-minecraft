#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>
#include <vector>

#include "raylib.h"
#include "block.hpp"

namespace World
{
    constexpr int CHUNK_SIZE = 8;
    constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    class Chunk
    {
    private:
        static int index(int x, int y, int z);
        bool isAir(int x, int y, int z) const;
        void getTileUV(BlockType type, Vector2 &uvMin, Vector2 &uvMax) const;
        
        std::array<Block, CHUNK_VOLUME> blocks;

    public:
        Chunk();

        Block getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, BlockType type);

        Mesh buildMesh() const;
    };
}

#endif