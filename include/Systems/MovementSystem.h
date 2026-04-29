#pragma once

#include "ECS/ECS.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionEvent.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BoxColliderComponent.h"

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(const std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
    }

    void onCollision(CollisionEvent &event)
    {
        Entity a = event.a;
        Entity b = event.b;

        Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

        if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles"))
        {
            OnEnemyHitsObstacle(a, b);
        }
        else if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies"))
        {
            OnEnemyHitsObstacle(b, a);
        }
    }

    void OnEnemyHitsObstacle(Entity enemy, Entity obstacle)
    {
        if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>())
        {
            auto &rigidBody = enemy.GetComponent<RigidBodyComponent>();
            auto &sprite = enemy.GetComponent<SpriteComponent>();

            if (rigidBody.velocity.x != 0)
            {
                rigidBody.velocity.x *= -1;
                sprite.assetId = (sprite.assetId == "tank-tiger-left-texture" ? "tank-tiger-right-texture" : "tank-tiger-left-texture");
            }
            if (rigidBody.velocity.y != 0)
            {
                rigidBody.velocity.y *= -1;
                sprite.assetId = (sprite.assetId == "tank-tiger-up-texture" ? "tank-tiger-down-texture" : "tank-tiger-up-texture");
            }
        }
    }

    void Update(double deltaTime)
    {
        // Loop all entities that the system is interested in
        for (auto entity : GetSystemEntities())
        {
            // Update entity position based on its velocity
            auto &transform = entity.GetComponent<TransformComponent>();
            const auto &rigidBody = entity.GetComponent<RigidBodyComponent>();
            const auto &box = entity.GetComponent<BoxColliderComponent>();

            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;

            // Prevent the main player from moving outside the map boundaries
            if (entity.HasTag("player"))
            {
                int paddingLeft = 10;
                int paddingTop = 10;
                int paddingRight = 50;
                int paddingBottom = 50;

                // Clamp X position between left and right map limits
                transform.position.x = SDL_clamp(transform.position.x, paddingLeft, Game::mapWidth - paddingRight);

                // Clamp Y position between top and bottom map limits
                transform.position.y = SDL_clamp(transform.position.y, paddingTop, Game::mapHeight - paddingBottom);
            }

            // Eliminate entities that are outside map boundaries
            float left = transform.position.x + box.offset.x * transform.scale.x;
            float top = transform.position.y + box.offset.y * transform.scale.y;
            float right = left + box.width * transform.scale.x;
            float bottom = top + box.height * transform.scale.y;

            bool isEntityOutsideMap = (right < 0 ||
                                       left > Game::mapWidth ||
                                       bottom < 0 ||
                                       top > Game::mapHeight);

            if (isEntityOutsideMap && !entity.HasTag("player"))
            {
                entity.Kill();
            }
        }
    }
};
