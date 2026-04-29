#pragma once

#include "ECS/ECS.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/HealthComponent.h"
#include "Components/DamageAnimationComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TextLabelComponent.h"
#include "Systems/RenderHealthBarSystem.h"
#include "Events/CollisionEvent.h"
#include "EventBus/EventBus.h"
#include <string>

class DamageSystem : public System
{
private:
    // Returns the destroyed texture ID for tanks/trucks, or empty string if not applicable
    std::string GetDestroyedTexture(const std::string &assetId)
    {
        // Tank Tiger: tank-tiger-up/right/down/left-texture -> tank-tiger-killed-texture
        if (assetId.find("tank-tiger-") != std::string::npos && assetId.find("-killed-") == std::string::npos)
        {
            return "tank-tiger-killed-texture";
        }
        // Tank Panther: tank-panther-up/right/down/left-texture -> tank-panther-killed-texture
        if (assetId.find("tank-panther-") != std::string::npos && assetId.find("-killed-") == std::string::npos)
        {
            return "tank-panther-killed-texture";
        }
        // Truck Ford: truck-ford-up/right/down/left-texture -> truck-ford-killed-texture
        if (assetId.find("truck-ford-") != std::string::npos && assetId.find("-killed-") == std::string::npos)
        {
            return "truck-ford-killed-texture";
        }
        // Army units
        if (assetId.find("army-walk-") != std::string::npos && assetId.find("-killed-") == std::string::npos)
        {
            return "army-walk-killed-texture";
        }
        if (assetId.find("army-gun-") != std::string::npos && assetId.find("-killed-") == std::string::npos)
        {
            return "army-gun-killed-texture";
        }

        return ""; // No destroyed texture available
    }

public:
    DamageSystem()
    {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent &event)
    {
        Entity a = event.a;
        Entity b = event.b;

        Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

        if (a.BelongsToGroup("projectiles") && b.HasTag("player"))
        {
            OnProjectHitsPlayer(a, b);
        }
        else if (b.BelongsToGroup("projectiles") && a.HasTag("player"))
        {
            OnProjectHitsPlayer(b, a);
        }
        else if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies"))
        {
            OnProjectHitsEnemy(a, b);
        }
        else if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies"))
        {
            OnProjectHitsEnemy(b, a);
        }
    }

    void OnProjectHitsPlayer(Entity projectile, Entity player)
    {
        auto &projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly) // Projectile emitted by enemy
        {
            // Reduce the health of the player by the projectile hitDamage
            auto &health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitDamage;

            if (player.HasComponent<DamageAnimationComponent>())
            {
                auto &blink = player.GetComponent<DamageAnimationComponent>();
                blink.StartBlink();
            }

            // Kills the player when health reaches zero
            if (health.healthPercentage <= 0)
            {
                player.Kill();
            }

            projectile.Kill();
        }
    }

    void OnProjectHitsEnemy(Entity projectile, Entity enemy)
    {
        auto &projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (projectileComponent.isFriendly) // Projectile emitted by player
        {
            // Reduce the health of the enemy by the projectile hitDamage
            auto &health = enemy.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitDamage;

            if (enemy.HasComponent<DamageAnimationComponent>())
            {
                auto &blink = enemy.GetComponent<DamageAnimationComponent>();
                blink.StartBlink();
            }

            // Kills the player when health reaches zero
            if (health.healthPercentage <= 0)
            {
                if (enemy.HasComponent<SpriteComponent>())
                {
                    auto &sprite = enemy.GetComponent<SpriteComponent>();
                    std::string killedTexture = GetDestroyedTexture(sprite.assetId);

                    if (!killedTexture.empty())
                    {
                        // Switch to destroyed texture
                        sprite.assetId = killedTexture;
                        sprite.srcRect.x = 0;
                        sprite.srcRect.y = 0;

                        // Remove from enemies group so it won't be targeted anymore
                        enemy.registry->RemoveEntityGroup(enemy);

                        // Remove interactive components - enemy becomes decoration
                        if (enemy.HasComponent<BoxColliderComponent>())
                            enemy.RemoveComponent<BoxColliderComponent>();
                        if (enemy.HasComponent<ProjectileEmitterComponent>())
                            enemy.RemoveComponent<ProjectileEmitterComponent>();
                        if (enemy.HasComponent<HealthComponent>())
                            enemy.RemoveComponent<HealthComponent>();
                        if (enemy.HasComponent<RigidBodyComponent>())
                            enemy.RemoveComponent<RigidBodyComponent>();
                        if (enemy.HasComponent<AnimationComponent>())
                            enemy.RemoveComponent<AnimationComponent>();
                        if (enemy.HasComponent<DamageAnimationComponent>())
                            enemy.RemoveComponent<DamageAnimationComponent>();
                        if (enemy.HasComponent<TextLabelComponent>())
                            enemy.RemoveComponent<TextLabelComponent>();

                        // Update system  so entity is removed from systems requiring removed components
                        enemy.registry->UpdateEntityInSystems(enemy);

                        Logger::Log("Entity " + std::to_string(enemy.GetId()) + " destroyed, texture switched to: " + killedTexture);
                    }
                    else
                    {
                        // No destroyed texture available, just kill the enemy
                        enemy.Kill();
                    }
                }
                else
                {
                    enemy.Kill();
                }
            }

            projectile.Kill();
        }
    }

    void Update()
    {
    }
};
