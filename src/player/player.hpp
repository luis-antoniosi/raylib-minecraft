#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"

namespace Game
{
    class Player
    {
    private:
        Vector2 lookRotation = {0.0f, 0.0f}; // yaw and pitch in radians
        float moveSpeed = 5.0f;
        Vector2 mouseSensivitity = {0.001f, 0.001f};
        float pitchLimit = 1.5f;

        void updateLook(float dt);
        void updateMove(float dt);

    public:
        Player();

        void update(float dt);

        Camera3D camera;
        Vector3 position;
    };
}

#endif