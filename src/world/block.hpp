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
        Grass,
        COUNT
    };

    struct Block
    {
        BlockType type = BlockType::Air;
    };

    struct BlockDefinition
    {
        Vector2 uvMin;
        Vector2 uvMax;
        bool solid;
    };

    class BlockRegistry
    {
    public:
        static const BlockDefinition &get(BlockType type)
        {
            static const std::array<BlockDefinition, static_cast<size_t>(BlockType::COUNT)> DEFINITIONS = {{
                {{0.0f, 0.0f}, {0.0f, 0.0f}, false}, // Air
                {{0.5f, 0.0f}, {1.0f, 1.0f}, true},  // Dirt
                {{0.0f, 0.0f}, {0.5f, 1.0f}, true},  // Stone
                {{0.0f, 0.0f}, {0.5f, 1.0f}, true}, // Grass
            }};

            return DEFINITIONS[static_cast<size_t>(type)];
        }
    };
}

#endif