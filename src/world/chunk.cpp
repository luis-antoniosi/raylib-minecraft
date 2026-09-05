#include <cstring>

#include "chunk.hpp"

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

    bool Chunk::isAir(int x, int y, int z) const
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
            return true;

        return blocks[index(x, y, z)].type == BlockType::Air;
    }

    void Chunk::getTileUV(BlockType type, Vector2 &uvMin, Vector2 &uvMax) const
    {
        const float inset = 0.5f / 32.0f; // atlas width

        switch (type)
        {
        case BlockType::Stone:
            uvMin = {0.0f + inset, 0.0f};
            uvMax = {0.5f - inset, 1.0f};
            break;
        case BlockType::Dirt:
            uvMin = {0.5f + inset, 0.0f};
            uvMax = {1.0f - inset, 1.0f};
            break;
        default:
            uvMin = {0.0f, 0.0f};
            uvMax = {1.0f, 1.0f};
            break;
        }
    }

    Mesh Chunk::buildMesh() const
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
                        if (!isAir(x + face.dx, y + face.dy, z + face.dz))
                            continue;

                        Vector2 uvMin, uvMax;
                        getTileUV(type, uvMin, uvMax);

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