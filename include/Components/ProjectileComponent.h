#pragma once

#include "SDL.h"

struct ProjectileComponent
{
    bool isFriendly;
    int hitDamage;
    Uint32 duration;
    Uint32 startTime;

    ProjectileComponent()
        : isFriendly(false), hitDamage(0), duration(0) {}

    ProjectileComponent(bool isFriendly, int hitDamage, int duration)
        : isFriendly(isFriendly), hitDamage(hitDamage), duration(duration), startTime(SDL_GetTicks()) {}
};
