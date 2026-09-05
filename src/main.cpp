#include <iostream>
#include <string>

#include "raylib.h"
#include "chunk.hpp"
#include "player.hpp"

#define HOST "127.0.0.1"
#define PORT 25565

// ip route show | grep default | awk '{print $3}'

void ApplyAtlasUV(Mesh &mesh, float uMin, float vMin, float uMax, float vMax);

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

    World::Chunk chunk;

    Mesh chunkMesh = chunk.buildMesh();
    Model chunkModel = LoadModelFromMesh(chunkMesh);
    chunkModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlas;

    Game::Player player;

    while (!WindowShouldClose())
    {
        player.update(GetFrameTime());
        // std::cout << "Player pos: " << player.position.x << ", " << player.position.y << ", " << player.position.z << std::endl;
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(player.camera);

        DrawModel(chunkModel, {0, 0, 0}, 1.0f, WHITE);

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    EnableCursor();
    CloseWindow();

    return 0;
}

void ApplyAtlasUV(Mesh &mesh, float uMin, float vMin, float uMax, float vMax)
{
    for (int i = 0; i < mesh.vertexCount; i++)
    {
        float u = mesh.texcoords[i * 2 + 0];
        float v = mesh.texcoords[i * 2 + 1];

        mesh.texcoords[i * 2 + 0] = uMin + u * (uMax - uMin);
        mesh.texcoords[i * 2 + 1] = vMin + v * (vMax - vMin);
    }

    UpdateMeshBuffer(mesh, 1, mesh.texcoords, mesh.vertexCount * 2 * sizeof(float), 0);
}