#pragma once

#include "EventBus/Event.h"

class KeyReleasedEvent : public Event
{
public:
    int keyCode;

    KeyReleasedEvent(int keyCode)
        : keyCode(keyCode) {}
};
