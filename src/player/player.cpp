#include "player.hpp"
#include "raymath.h"

namespace Game
{
    Player::Player()
    {
        position = (Vector3){9.0f, 5.0f, 3.0f};

        camera.position = position;
        camera.target = Vector3Add(position, {0.0f, 0.0f, -1.0f});
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void Player::updateLook()
    {
        Vector2 mouseDelta = GetMouseDelta();

        lookRotation.x += mouseDelta.x * mouseSensivitity.x;
        lookRotation.y -= mouseDelta.y * mouseSensivitity.y;

        lookRotation.y = Clamp(lookRotation.y, -pitchLimit, pitchLimit);
    }

    void Player::updateMove(float dt)
    {
        Vector3 forward;
        forward.x = sinf(lookRotation.x) * cosf(lookRotation.y);
        forward.y = sinf(lookRotation.y);
        forward.z = -cosf(lookRotation.x) * cosf(lookRotation.y);
        forward = Vector3Normalize(forward);

        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
        Vector3 moveDir = {0};

        if (IsKeyDown(KEY_W))
            moveDir = Vector3Add(moveDir, forward);
        if (IsKeyDown(KEY_S))
            moveDir = Vector3Subtract(moveDir, forward);
        if (IsKeyDown(KEY_D))
            moveDir = Vector3Add(moveDir, right);
        if (IsKeyDown(KEY_A))
            moveDir = Vector3Subtract(moveDir, right);
        if (IsKeyDown(KEY_SPACE))
            moveDir = Vector3Add(moveDir, camera.up);
        if (IsKeyDown(KEY_LEFT_SHIFT))
            moveDir = Vector3Subtract(moveDir, camera.up);

        if (Vector3Length(moveDir) > 0.0f)
        {
            moveDir = Vector3Normalize(moveDir);
            position = Vector3Add(position, Vector3Scale(moveDir, moveSpeed * dt));
        }

        camera.position = position;
        camera.target = Vector3Add(position, forward);
    }

    void Player::update(float dt)
    {
        updateLook();
        updateMove(dt);
    }
}