#pragma once

#include "ECS/ECS.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/DamageAnimationComponent.h"
#include "AssetStore/AssetStore.h"
#include "SDL.h"

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera)
    {
        // Collect entities
        std::vector<Entity> entitiesToRender;

        for (auto &entity : GetSystemEntities())
        {
            entitiesToRender.emplace_back(entity);
        }

        // Sort by z-Index
        std::stable_sort(entitiesToRender.begin(), entitiesToRender.end(), [](const Entity &a, const Entity &b)
                         { 
                    auto &spriteA = a.GetComponent<SpriteComponent>(); 
                    auto &spriteB = b.GetComponent<SpriteComponent>();
                    return spriteA.zIndex < spriteB.zIndex; });

        // Loop all entities that the system is interested in
        for (const auto &entity : entitiesToRender)
        {

            // Check if entity should be rendered
            if (entity.HasComponent<DamageAnimationComponent>())
            {
                const auto &blink = entity.GetComponent<DamageAnimationComponent>();

                //  Skip rendering if entity is blinking and currently invisible
                if (!blink.isVisible)
                {
                    continue;
                }
            }

            // Update entity position based on its velocity
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &sprite = entity.GetComponent<SpriteComponent>();

            // Set the source rectangle of our original sprite texture
            SDL_Rect srcRect = sprite.srcRect;

            // Set the destination rectangle with the x,y position to be rendered
            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)};

            // Cull sprites outside the camera view
            bool isOutsideCamera = (transform.position.x + sprite.width * transform.scale.x < camera.x ||
                                    transform.position.x > camera.x + camera.w ||
                                    transform.position.y + sprite.height * transform.scale.y < camera.y ||
                                    transform.position.y > camera.y + camera.h);

            if (isOutsideCamera && !sprite.isFixed)
            {
                continue;
            }

            // Render the entity's sprite to the screen
            SDL_RenderCopyEx(renderer,
                             assetStore->GetTexture(sprite.assetId),
                             &srcRect,
                             &dstRect,
                             transform.rotation,
                             nullptr,
                             SDL_FLIP_NONE);
        }
    }
};
