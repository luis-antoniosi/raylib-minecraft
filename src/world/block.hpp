#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "raylib.h"

namespace World
{
    enum class BlockType
    {
        Air,
        Dirt,
        Stone,
        Grass
    };

    class Block
    {
        public:
            BlockType type;
    };
}

#endif