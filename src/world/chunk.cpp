#include <cstring>

#include "chunk.hpp"
#include "world.hpp"

namespace World
{
    struct FaceData
    {
        Vector3 offsets[4];
        Vector3 normal;
        int dx, dy, dz;
    };

    static const FaceData FACES[6] = {
        // +X
        {{{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}}, {1, 0, 0}, 1, 0, 0},
        // -X
        {{{0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0}}, {-1, 0, 0}, -1, 0, 0},
        // +Y
        {{{0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}}, {0, 1, 0}, 0, 1, 0},
        // -Y
        {{{0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}}, {0, -1, 0}, 0, -1, 0},
        // +Z
        {{{1, 0, 1}, {1, 1, 1}, {0, 1, 1}, {0, 0, 1}}, {0, 0, 1}, 0, 0, 1},
        // -Z
        {{{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}}, {0, 0, -1}, 0, 0, -1},
    };

    Chunk::Chunk()
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    int idx = index(x, y, z);
                    if (y < 4)
                        blocks[idx].type = BlockType::Stone;
                    else if (y < 6)
                        blocks[idx].type = BlockType::Dirt;
                    else
                        blocks[idx].type = BlockType::Air;
                }
            }
        }
    }

    int Chunk::index(int x, int y, int z)
    {
        return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
    }

    Block Chunk::getBlock(int x, int y, int z)
    {
        return blocks[index(x, y, z)];
    }

    void Chunk::setBlock(int x, int y, int z, BlockType type)
    {
        blocks[index(x, y, z)].type = type;
    }

    bool Chunk::isAirWorld(World &world, int chunkX, int chunkZ, int x, int y, int z) const
    {
        if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)
            return blocks[index(x, y, z)].type == BlockType::Air;

        int worldX = chunkX * CHUNK_SIZE + x;
        int worldZ = chunkZ * CHUNK_SIZE + z;

        return world.getBlockWorld(worldX, y, worldZ) == BlockType::Air;
    }

    Mesh Chunk::buildMesh(World &world, int chunkX, int chunkZ) const
    {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<unsigned short> indices;

        unsigned short vertexCount = 0;

        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    BlockType type = blocks[index(x, y, z)].type;
                    if (type == BlockType::Air)
                        continue;

                    for (const FaceData &face : FACES)
                    {
                        // if the neighboring block is not air, its not visible, so skip it.
                        if (!isAirWorld(world, chunkX, chunkZ, x + face.dx, y + face.dy, z + face.dz))
                            continue;

                        const BlockDefinition &def = BlockRegistry::get(type);
                        Vector2 uvMin = def.uvMin;
                        Vector2 uvMax = def.uvMax;

                        Vector2 faceUVs[4] =
                            {
                                {uvMin.x, uvMax.y},
                                {uvMax.x, uvMax.y},
                                {uvMax.x, uvMin.y},
                                {uvMin.x, uvMin.y},
                            };

                        for (int i = 0; i < 4; i++)
                        {
                            vertices.push_back(x + face.offsets[i].x);
                            vertices.push_back(y + face.offsets[i].y);
                            vertices.push_back(z + face.offsets[i].z);

                            normals.push_back(face.normal.x);
                            normals.push_back(face.normal.y);
                            normals.push_back(face.normal.z);

                            uvs.push_back(faceUVs[i].x);
                            uvs.push_back(faceUVs[i].y);
                        }

                        indices.push_back(vertexCount + 0);
                        indices.push_back(vertexCount + 1);
                        indices.push_back(vertexCount + 2);
                        indices.push_back(vertexCount + 0);
                        indices.push_back(vertexCount + 2);
                        indices.push_back(vertexCount + 3);

                        vertexCount += 4;
                    }
                }
            }
        }

        Mesh mesh = {0};
        mesh.vertexCount = vertexCount;
        mesh.triangleCount = static_cast<int>(indices.size() / 3);

        mesh.vertices = (float *)MemAlloc(vertices.size() * sizeof(float));
        memcpy(mesh.vertices, vertices.data(), vertices.size() * sizeof(float));

        mesh.normals = (float *)MemAlloc(normals.size() * sizeof(float));
        memcpy(mesh.normals, normals.data(), normals.size() * sizeof(float));

        mesh.texcoords = (float *)MemAlloc(uvs.size() * sizeof(float));
        memcpy(mesh.texcoords, uvs.data(), uvs.size() * sizeof(float));

        mesh.indices = (unsigned short *)MemAlloc(indices.size() * sizeof(unsigned short));
        memcpy(mesh.indices, indices.data(), indices.size() * sizeof(unsigned short));

        UploadMesh(&mesh, false);
        return mesh;
    }
}