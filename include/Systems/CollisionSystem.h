#pragma once

#include <unordered_set>
#include "ECS/ECS.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TransformComponent.h"
#include "Events/CollisionEvent.h"

class CollisionSystem : public System
{
public:
    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(std::unique_ptr<EventBus> &eventBus)
    {
        auto &entities = GetSystemEntities();

        // Loop all the entities that the system is interested in
        for (size_t i = 0; i < entities.size(); i++)
        {
            auto &a = entities[i];
            auto &transformA = a.GetComponent<TransformComponent>();
            auto &colliderA = a.GetComponent<BoxColliderComponent>();

            glm::vec2 posA = transformA.position + colliderA.offset * transformA.scale;
            glm::vec2 sizeA(colliderA.width * transformA.scale.x, colliderA.height * transformA.scale.y);

            // Loop all the entities to check for collision with a (to the right of i) using AABB collision
            for (size_t j = i + 1; j < entities.size(); j++)
            {
                auto &b = entities[j];
                auto &transformB = b.GetComponent<TransformComponent>();
                auto &colliderB = b.GetComponent<BoxColliderComponent>();

                glm::vec2 posB = transformB.position + colliderB.offset * transformB.scale;
                glm::vec2 sizeB(colliderB.width * transformB.scale.x, colliderB.height * transformB.scale.y);

                bool collisionX = posA.x < posB.x + sizeB.x && posA.x + sizeA.x > posB.x;
                bool collisionY = posA.y < posB.y + sizeB.y && posA.y + sizeA.y > posB.y;
                bool collisionHappened = collisionX && collisionY;

                if (collisionHappened)
                {
                    eventBus->EmitEvent<CollisionEvent>(a, b);
                }
            }
        }
    }

    // Draw box colliders
    void Debug(SDL_Renderer *renderer, SDL_Rect &camera)
    {
        for (auto &entity : GetSystemEntities())
        {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &collider = entity.GetComponent<BoxColliderComponent>();

            SDL_Rect colliderRect = {
                static_cast<int>(transform.position.x + collider.offset.x - camera.x),
                static_cast<int>(transform.position.y + collider.offset.y - camera.y),
                static_cast<int>(collider.width * transform.scale.x),
                static_cast<int>(collider.height * transform.scale.y)};

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
};
