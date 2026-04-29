#pragma once

#include <glm.hpp>

struct KeyboardControlledComponent
{
    float speed;

    // Key states
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    // Velocities
    glm::vec2 upVelocity;
    glm::vec2 rightVelocity;
    glm::vec2 downVelocity;
    glm::vec2 leftVelocity;

    KeyboardControlledComponent(float speed = 0.0f)
        : speed(speed)
    {
        upVelocity = {0, -speed};
        downVelocity = {0, speed};
        leftVelocity = {-speed, 0};
        rightVelocity = {speed, 0};
    }
};
