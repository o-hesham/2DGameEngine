#pragma once

#include <SDL.h>

struct DamageAnimationComponent
{
    bool isBlinking;
    int blinkCount;
    int maxBlinks;           // Number of full blinks (3 = 6 toggles)
    Uint32 blinkOnDuration;  // How long sprite is VISIBLE
    Uint32 blinkOffDuration; // How long sprite is INVISIBLE
    Uint32 lastToggleTime;
    bool isVisible;

    DamageAnimationComponent(
        int numBlinks = 3,           // 3 full blinks
        Uint32 blinkOnDuration = 80, // Visible for 80ms
        Uint32 blinkOffDuration = 40 // Invisible for 40ms
        )
        : isBlinking(false),
          blinkCount(0),
          maxBlinks(numBlinks * 2), // 3 blinks = 6 toggles
          blinkOnDuration(blinkOnDuration),
          blinkOffDuration(blinkOffDuration),
          lastToggleTime(0),
          isVisible(true)
    {
    }

    void StartBlink()
    {
        isBlinking = true;
        blinkCount = 0;
        isVisible = false;
        lastToggleTime = SDL_GetTicks();
    }

    void StopBlink()
    {
        isBlinking = false;
        blinkCount = 0;
        isVisible = true;
    }
};