#pragma once

#include <string>
#include "ECS/ECS.h"
#include "EventBus/EventBus.h"
#include "Events/KeyPressedEvent.h"
#include "Events/KeyReleasedEvent.h"
#include "Components/KeyboardControlledComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/RigidBodyComponent.h"

class KeyBoardControlSystem : public System
{
public:
    KeyBoardControlSystem()
    {
        RequireComponent<KeyboardControlledComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyBoardControlSystem::OnKeyPress);
        eventBus->SubscribeToEvent<KeyReleasedEvent>(this, &KeyBoardControlSystem::OnKeyRelease);
    }

    void OnKeyPress(KeyPressedEvent &event)
    {
        for (auto entity : GetSystemEntities())
        {
            auto &control = entity.GetComponent<KeyboardControlledComponent>();

            switch (event.keyCode)
            {
            case SDLK_w:
                control.up = true;
                break;
            case SDLK_a:
                control.left = true;
                break;
            case SDLK_s:
                control.down = true;
                break;
            case SDLK_d:
                control.right = true;
                break;
            }
        }
    }

    void OnKeyRelease(KeyReleasedEvent &event)
    {
        for (auto entity : GetSystemEntities())
        {
            auto &control = entity.GetComponent<KeyboardControlledComponent>();

            switch (event.keyCode)
            {
            case SDLK_w:
                control.up = false;
                break;
            case SDLK_a:
                control.left = false;
                break;
            case SDLK_s:
                control.down = false;
                break;
            case SDLK_d:
                control.right = false;
                break;
            }
        }
    }

    void Update()
    {
        for (auto entity : GetSystemEntities())
        {
            auto &control = entity.GetComponent<KeyboardControlledComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();
            auto &rigid = entity.GetComponent<RigidBodyComponent>();

            glm::vec2 v = {0, 0};

            if (control.up)
                v.y -= control.speed;
            else if (control.down)
                v.y += control.speed;
            else if (control.left)
                v.x -= control.speed;
            else if (control.right)
                v.x += control.speed;

            rigid.velocity = v;

            // Update animation based on direction
            if (v.y < 0)
                sprite.srcRect.y = sprite.height * 0; // up
            else if (v.x > 0)
                sprite.srcRect.y = sprite.height * 1; // right
            else if (v.y > 0)
                sprite.srcRect.y = sprite.height * 2; // down
            else if (v.x < 0)
                sprite.srcRect.y = sprite.height * 3; // left
        }
    }
};
