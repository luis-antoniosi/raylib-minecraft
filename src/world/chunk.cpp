#include "chunk.hpp"

namespace World
{
    Chunk::Chunk()
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    blocks[index(x, y, z)] = (y < 4) ? BlockType::Stone : BlockType::Air;
                }
            }
        }
    }

    BlockType Chunk::getBlock(int x, int y, int z)
    {
        return blocks[index(x, y, z)];
    }

    void Chunk::setBlock(int x, int y, int z, BlockType type)
    {
        blocks[index(x, y, z)] = type;
    }
}