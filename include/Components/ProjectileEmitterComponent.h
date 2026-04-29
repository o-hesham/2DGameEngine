#pragma once

#include <glm.hpp>

struct ProjectileEmitterComponent
{
    glm::vec2 projectileVelocity;
    Uint32 repeatFrquency;
    int projectileDuration;
    int hitDamage;
    Uint32 lastEmissionTime;
    bool isFriendly;

    ProjectileEmitterComponent()
        : projectileVelocity(0), repeatFrquency(0), projectileDuration(10000), hitDamage(10), isFriendly(false) {}

    ProjectileEmitterComponent(glm::vec2 projectileVelocity, int repeatFrquency, int projectileDuration, int hitDamage, bool isFriendly)
        : projectileVelocity(projectileVelocity), repeatFrquency(repeatFrquency), projectileDuration(projectileDuration), hitDamage(hitDamage), lastEmissionTime(0), isFriendly(isFriendly) {}
};
