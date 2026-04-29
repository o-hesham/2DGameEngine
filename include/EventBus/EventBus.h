#pragma once

#include "Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <memory>
#include <list>
#include <functional>

class IEventCallBack
{
private:
    virtual void Call(Event &e) = 0;

public:
    virtual ~IEventCallBack() = default;

    void Execute(Event &e)
    {
        Call(e);
    }
};

// TOwner: The type of the class that owns the callback
// TEvent: The event type the method responds to
template <typename TOwner, typename TEvent>
class EventCallBack : public IEventCallBack
{
private:
    // This is a member function pointer type
    // It represents functions like: void Player::OnKeyPressed(KeyPressedEvent&);
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *ownerInstance;
    CallbackFunction callbackFunction;

    // Call the member function callbackFunction on the object ownerInstance, and pass the event converted to TEvent.
    virtual void Call(Event &e) override
    {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent &>(e));
    }

public:
    EventCallBack(TOwner *ownerInstance, CallbackFunction callbackFunction)
        : ownerInstance(ownerInstance), callbackFunction(callbackFunction) {}

    ~EventCallBack() override = default;
};

typedef std::list<std::unique_ptr<IEventCallBack>> HandlerList;

class EventBus
{
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus() { Logger::Log("EventBus constructor called!"); };

    ~EventBus() { Logger::Log("EventBus destructor called!"); };

    // Clears the subscribers list
    void Reset()
    {
        subscribers.clear();
    }

    /////////////////////////////////////////////////////////////////////////////
    // Subscribe to an event type <T>
    // In our implementation, a listener subscribes to an event
    // Example: eventBus->SubscribeToEvent<CollisionEvent>(this, &Game::onCollision);
    /////////////////////////////////////////////////////////////////////////////
    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner *ownerInstance, void (TOwner::*callbackFunction)(TEvent &))
    {
        if (subscribers[typeid(TEvent)].get() == nullptr) // Does this event type already have a list of callbacks?
        {
            // If not create a new empty list!
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }

        // Make a new callback object that knows who owns the function (ownerInstance) and what function to call (callbackFunction)
        auto subscriber = std::make_unique<EventCallBack<TOwner, TEvent>>(ownerInstance, callbackFunction);
        // Add this callback to the list of handlers for this event type
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    /////////////////////////////////////////////////////////////////////////////
    // Emit an event of type <T>
    // In our implementation, as soon as something emits an event we go ahead
    // and execute all the listener callback functions.
    // Example: eventBus->EmitEvent<CollisionEvent>(player, enemy);
    /////////////////////////////////////////////////////////////////////////////
    template <typename TEvent, typename... TArgs>
    void EmitEvent(TArgs &&...args)
    {
        // Get the list of handlers (subscribers) for this event type.
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers)
        {
            // Loop through all subscribed handlers in the list
            for (auto it = handlers->begin(); it != handlers->end(); it++)
            {
                auto handler = it->get();
                TEvent event(std::forward<TArgs>(args)...); // Construct a new event object using the forwarded arguments.
                handler->Execute(event);                    // Call the callback associated with this handler.
            }
        }
    }
};
