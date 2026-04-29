#pragma once

#include <glm.hpp>
#include "ECS/ECS.h"
#include "Components/CameraFollowComponent.h"
#include "Components/TransformComponent.h"

class CameraMovementSystem : public System
{
public:
    CameraMovementSystem()
    {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Rect &camera)
    {
        auto entity = GetSystemEntities();
        if (entity.empty())
        {
            return;
        }

        const auto &transform = entity.front().GetComponent<TransformComponent>();

        // Center the camera on the entity
        camera.x = transform.position.x - camera.w / 2;
        camera.y = transform.position.y - camera.h / 2;

        // Clamp to map boundaries; works if map smaller than camera
        camera.x = static_cast<int>(
            glm::clamp(static_cast<float>(camera.x), 0.0f, static_cast<float>(std::max(Game::mapWidth - camera.w, 0))));
        camera.y = static_cast<int>(
            glm::clamp(static_cast<float>(camera.y), 0.0f, static_cast<float>(std::max(Game::mapHeight - camera.h, 0))));
    }
};