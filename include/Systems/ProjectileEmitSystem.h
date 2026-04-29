#pragma once

#include "SDL.h"
#include "ECS/ECS.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileComponent.h"

class ProjectileEmitSystem : public System
{
private:
    void EmitProjectile(Registry &registry, const glm::vec2 &startPos, const glm::vec2 &velocity, const ProjectileEmitterComponent &emitter)
    {
        Entity projectile = registry.CreateEntity();
        projectile.Group("projectiles");
        projectile.AddComponent<TransformComponent>(startPos, glm::vec2(1.0f, 1.0f), 0.0);
        projectile.AddComponent<RigidBodyComponent>(velocity);
        projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
        projectile.AddComponent<BoxColliderComponent>(4, 4);
        projectile.AddComponent<ProjectileComponent>(emitter.isFriendly, emitter.hitDamage, emitter.projectileDuration);
    }

    glm::vec2 GetProjectileStartPosition(const TransformComponent &transform, const SpriteComponent &sprite)
    {
        auto projectilePosition = transform.position;
        projectilePosition.x += transform.scale.x * sprite.width / 2;
        projectilePosition.y += transform.scale.y * sprite.height / 2;

        return projectilePosition;
    }

public:
    ProjectileEmitSystem()
    {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::onKeyPress);
    }

    // ---------------------------------------------------------
    //  MANUAL SHOOTING (Space key)
    // ---------------------------------------------------------
    void onKeyPress(KeyPressedEvent &event)
    {
        if (event.keyCode != SDLK_SPACE)
            return;

        for (auto entity : GetSystemEntities())
        {
            if (entity.HasComponent<CameraFollowComponent>())
            {
                const auto &emitter = entity.GetComponent<ProjectileEmitterComponent>();
                const auto &transform = entity.GetComponent<TransformComponent>();
                const auto &sprite = entity.GetComponent<SpriteComponent>();

                // Determine direction based on sprite frame (0=up,1=right,2=down,3=left)
                glm::vec2 direction;
                switch (sprite.srcRect.y / sprite.height)
                {
                case 0:
                    direction = {0, -1};
                    break; // up
                case 1:
                    direction = {1, 0};
                    break; // right
                case 2:
                    direction = {0, 1};
                    break; // down
                case 3:
                    direction = {-1, 0};
                    break; // left
                default:
                    direction = {1, 0};
                    break; // default right
                }

                // Projectile start position (center of chopper)
                auto projectilePosition = GetProjectileStartPosition(transform, sprite);
                auto projectileVelocity = direction * emitter.projectileVelocity;

                // Create new projectile
                EmitProjectile(*entity.registry, projectilePosition, projectileVelocity, emitter);
            }
        }
    }

    // ---------------------------------------------------------
    //  AUTO-REPEAT SHOOTING
    // ---------------------------------------------------------
    void Update()
    {
        for (auto entity : GetSystemEntities())
        {
            auto &emitter = entity.GetComponent<ProjectileEmitterComponent>();
            const auto &transform = entity.GetComponent<TransformComponent>();

            if (emitter.repeatFrquency == 0)
            {
                continue;
            }

            // Check if its time to re-emit a new projectile
            if (SDL_GetTicks() - emitter.lastEmissionTime > emitter.repeatFrquency)
            {
                glm::vec2 projectilePosition;

                // If parent entity has sprite, start the projectile position in the middle
                if (entity.HasComponent<SpriteComponent>())
                {
                    auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition = GetProjectileStartPosition(transform, sprite);
                }
                else
                {
                    projectilePosition = transform.position;
                }

                // Add a new projectile entity to the registry
                EmitProjectile(*entity.registry, projectilePosition, emitter.projectileVelocity, emitter);

                // Update the projectile emitter component last emission to the current ms
                emitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};