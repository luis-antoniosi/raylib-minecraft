#include <iostream>
#include <string>
#include <thread>

#include "raylib.h"
#include "chunk.hpp"
#include "player.hpp"
#include "world.hpp"

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

    Texture2D atlas = LoadTexture("assets/atlas.png");
    if (atlas.id == 0)
        std::cout << "Failed to load atlas texture" << std::endl;
    else
        std::cout << "Atlas loaded: " << atlas.width << ", " << atlas.height << std::endl;

    World::World world;
    std::vector<Model> chunkModels;
    std::vector<Vector3> chunkPositions;

    const int RENDER_DISTANCE = 6;

    for (int cx = 0; cx < RENDER_DISTANCE; cx++)
    {
        for (int cz = 0; cz < RENDER_DISTANCE; cz++)
            world.getOrCreateChunk(cx, cz);
    }

    for (int cx = 0; cx < RENDER_DISTANCE; cx++)
    {
        for (int cz = 0; cz < RENDER_DISTANCE; cz++)
        {
            World::Chunk &chunk = world.getOrCreateChunk(cx, cz);
            Mesh mesh = chunk.buildMesh(world, cx, cz);

            Model model = LoadModelFromMesh(mesh);
            model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlas;

            chunkModels.push_back(model);
            chunkPositions.push_back(
                {(float)(cx * World::CHUNK_SIZE),
                 0.0f,
                 (float)(cz * World::CHUNK_SIZE)});
        }
    }

    Game::Player player;

    while (!WindowShouldClose())
    {
        player.update(GetFrameTime());
        // std::cout << "Player pos: " << player.position.x << ", " << player.position.y << ", " << player.position.z << std::endl;
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(player.camera);

        for (size_t i = 0; i < chunkModels.size(); i++)
        {
            DrawModel(chunkModels[i], chunkPositions[i], 1.0f, WHITE);

            DrawCubeWires(
                {chunkPositions[i].x + World::CHUNK_SIZE / 2.0f,
                 World::CHUNK_SIZE / 2.0f,
                 chunkPositions[i].z + World::CHUNK_SIZE / 2.0f},
                (float)World::CHUNK_SIZE,
                (float)World::CHUNK_SIZE,
                (float)World::CHUNK_SIZE,
                RED);
        }

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    EnableCursor();
    CloseWindow();

    return 0;
}