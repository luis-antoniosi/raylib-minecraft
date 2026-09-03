#include <iostream>
#include <string>

#include "raylib.h"
#include "chunk.hpp"
#include "player.hpp"

#define HOST "127.0.0.1"
#define PORT 25565

// ip route show | grep default | awk '{print $3}'

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "hello!");
    SetTargetFPS(60);
    DisableCursor();

    World::Chunk chunk;
    Game::Player player;

    while (!WindowShouldClose())
    {
        player.update(GetFrameTime());
        std::cout << "Player pos: " << player.position.x << ", " << player.position.y << ", " << player.position.z << std::endl;
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(player.camera);

        for (int x = 0; x < World::CHUNK_SIZE; x++)
        {
            for (int y = 0; y < World::CHUNK_SIZE; y++)
            {
                for (int z = 0; z < World::CHUNK_SIZE; z++)
                {
                    if (chunk.getBlock(x, y, z) != World::BlockType::Air)
                    {
                        Vector3 pos = {(float)x, (float)y, (float)z};
                        DrawCube(pos, 1.f, 1.f, 1.f, GRAY);
                        DrawCubeWires(pos, 1.f, 1.f, 1.f, DARKGRAY);
                    }
                }
            }
        }

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    EnableCursor();
    CloseWindow();

    return 0;
}