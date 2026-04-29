#pragma once

#include "EventBus/Event.h"
#include "SDL.h"

class KeyPressedEvent : public Event
{
public:
    SDL_Keycode keyCode;

    KeyPressedEvent(SDL_Keycode keyCode)
        : keyCode(keyCode) {}
};
