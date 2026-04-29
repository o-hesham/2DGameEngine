#pragma once

#include "ECS/ECS.h"
#include "Components/DamageAnimationComponent.h"
#include "Components/SpriteComponent.h"
#include <SDL.h>

class DamageAnimationSystem : public System
{
public:
    DamageAnimationSystem()
    {
        RequireComponent<DamageAnimationComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update()
    {
        for (auto entity : GetSystemEntities())
        {
            auto &blink = entity.GetComponent<DamageAnimationComponent>();

            if (!blink.isBlinking)
            {
                // Ensure visibility when not blinking
                if (!blink.isVisible)
                {
                    blink.isVisible = true;
                }
                continue;
            }

            Uint32 currentTime = SDL_GetTicks();

            // Use different duration based on current visibility
            Uint32 currentDuration = blink.isVisible
                                         ? blink.blinkOnDuration
                                         : blink.blinkOffDuration;

            Uint32 timeSinceLastToggle = currentTime - blink.lastToggleTime;

            if (timeSinceLastToggle >= currentDuration)
            {
                // Toggle visibility
                blink.isVisible = !blink.isVisible;
                blink.blinkCount++;
                blink.lastToggleTime = currentTime;

                // Check if all blinks complete
                if (blink.blinkCount >= blink.maxBlinks)
                {
                    blink.StopBlink();
                }
            }
        }
    }
};