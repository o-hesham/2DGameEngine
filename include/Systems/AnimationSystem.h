#pragma once

#include "ECS/ECS.h"
#include <SDL.h>
#include "Components/AnimationComponent.h"
#include "Components/SpriteComponent.h"

class AnimationSystem : public System
{
public:
    AnimationSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<AnimationComponent>();
    }

    void Update()
    {
        for (auto &entity : GetSystemEntities())
        {
            auto &animation = entity.GetComponent<AnimationComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            // Calculate which animation frame should be displayed based on elapsed time, speed, and frame count, and loop it smoothly
            if (animation.isLoop)
            {
                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
            }
            else
            {
                animation.currentFrame = std::min(static_cast<int>(((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000)), (animation.numFrames - 1));
            }
            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }
};
