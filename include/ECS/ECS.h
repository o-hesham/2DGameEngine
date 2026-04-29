#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include <cassert>
#include "Logger.h"

const unsigned int MAX_COMPONENTS = 32;

////////////////////////////////////////////////////////////////////////////////
// Signature
////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of which entities a systems is interested in
////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent
{
protected:
    inline static size_t nextID = 0;
};

// Used to assign a unique id to a componenet type
template <typename T>
class Component : public BaseComponent
{
public:
    // Returns the unique id of component<T>
    static size_t GetID()
    {
        static auto id = nextID++;
        return id;
    }
};

class Entity
{
private:
    size_t id;

public:
    Entity(size_t id)
        : id(id) {}
    Entity(const Entity &entity) = default;
    void Kill() const;
    size_t GetId() const { return id; }

    // Manage entity tags and groups
    void Tag(const std::string &tag);
    bool HasTag(const std::string &tag) const;
    void Group(const std::string &group);
    bool BelongsToGroup(const std::string &group) const;

    // Operator overloading for entity objects
    Entity &operator=(const Entity &rhs) = default;
    bool operator==(const Entity &rhs) const { return id == rhs.id; }
    bool operator!=(const Entity &rhs) const { return id != rhs.id; }
    bool operator>(const Entity &rhs) const { return id > rhs.id; }
    bool operator<(const Entity &rhs) const { return id < rhs.id; }

    // Manage entity components
    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs &&...args);
    template <typename TComponent>
    void RemoveComponent();
    template <typename TComponent>
    bool HasComponent() const;
    template <typename TComponent>
    TComponent &GetComponent() const;

    // Hold a pointer to the entity's owner register
    class Registry *registry;
};

////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////
class System
{
private:
    Signature componentSignature; // Components required by the system
    std::vector<Entity> entities; // List of entities (that contain the components required by the system) the system is interested in

public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    const std::vector<Entity> &GetSystemEntities() const;
    const Signature &GetComponenetSignature() const;

    // Defines the component type that entities must have to be considered by the system
    template <typename TComponent>
    void RequireComponent();
};

////////////////////////////////////////////////////////////////////////////////
// Pool
////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of objects of type T
////////////////////////////////////////////////////////////////////////////////
class IPool
{
public:
    virtual ~IPool() = default;
    virtual void RemoveEntityFromPool(int entityId) = 0;
};

template <typename T>
class Pool : public IPool
{
private:
    // We keep track of the vector of objects and the current number of elements
    std::vector<T> data;
    size_t size;

    // Helper maps to keep track of entity ids per index, so the vector is always packed
    std::unordered_map<int, int> entityIdToIndex;
    std::unordered_map<int, int> indexToEntityId;

public:
    Pool(size_t capacity = 100)
    {
        size = 0;
        data.resize(capacity);
    }
    ~Pool() override = default;

    bool isEmpty() const { return size == 0; }
    void Clear()
    {
        data.clear();
        size = 0;
    }

    size_t GetSize() const { return size; }
    void Resize(unsigned int n) { data.resize(n); }

    void Add(T object) { data.push_back(object); }

    void Set(unsigned int entityId, T object)
    {
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
        {
            // If the element already exits, simply replace the component object
            size_t index = entityIdToIndex[entityId];
            data[index] = object;
        }
        else
        {
            // When adding a new object, we keep track of the entity ids and their vector index
            size_t index = size;
            entityIdToIndex.emplace(entityId, index);
            indexToEntityId.emplace(index, entityId);
            if (index >= data.capacity())
            {
                // If necessary, we resize by always doubling the current capacity
                data.resize(size * 2);
            }
            data[index] = object;
            size++;
        }
    }

    void Remove(int entityId)
    {
        // Copy the last element to the deleted position to keep the array packed
        int indexOfRemoved = entityIdToIndex[entityId];
        int indexOfLast = size - 1;
        data[indexOfRemoved] = data[indexOfLast];

        // Update the index-entity maps to point to the correct elements
        int entityIdOfLastElement = indexToEntityId[indexOfLast];
        entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
        indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

        entityIdToIndex.erase(entityId);
        indexToEntityId.erase(indexOfLast);

        size--;
    }

    void RemoveEntityFromPool(int entityId) override
    {
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
        {
            Remove(entityId);
        }
    }

    T &Get(size_t entityId)
    {
        size_t index = entityIdToIndex[entityId];
        assert(index < data.size());
        return data[index];
    }

    T &operator[](size_t index) { return data[index]; }
};

////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems,
// and components
////////////////////////////////////////////////////////////////////////////////
class Registry
{
private:
    size_t numEntities = 0;

    // Vector of component pools, each pool contains all the data for a certain component type
    // Vector index = component type id
    // Pool index = entity id
    std::vector<std::unique_ptr<IPool>> componentPools;

    // Vector of component signatures per entity, saying which component is turned on for a given entity
    // Vector index = entity id
    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, std::unique_ptr<System>> systems;

    // Set of entities that are flagged to be added or removed in the next Update()
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

    // Entity tags (one tag name per entity)
    std::unordered_map<std::string, Entity> entityPerTag;
    std::unordered_map<int, std::string> tagpPerEntity;

    // Entity groups ( a set of entities per group name)
    std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
    std::unordered_map<int, std::string> groupPerEntity;

    // List of free entity ids that were previously removed
    std::vector<int> freeIds;

public:
    Registry()
    {
        Logger::Log("Registry constructor called!");
    }
    ~Registry() { Logger::Log("Registry destructor called!"); }

    // The registry Update() finally processes the entities that are waiting to be added/killed to the systems
    void Update();

    // Entity management
    Entity CreateEntity();
    void KillEntity(Entity entity);

    // Tag Management
    void TagEntity(Entity entity, const std::string &tag);
    bool EntityHasTag(Entity entity, const std::string &tag) const;
    Entity GetEntityByTag(const std::string &tag) const;
    void RemoveEntityTag(Entity entity);

    // Group Management
    void GroupEntity(Entity entity, const std::string &group);
    bool EntityBelongsToGroup(Entity entity, const std::string &group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string &group) const;
    void RemoveEntityGroup(Entity entity);

    // Component management
    template <typename TComponent, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&...args);
    template <typename TComponent>
    void RemoveComponent(Entity entity);
    template <typename TComponent>
    bool HasComponent(Entity entity) const;
    template <typename TComponent>
    TComponent &GetComponent(Entity entity) const;

    // System management
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs &&...args);
    template <typename TSystem>
    void RemoveSystem();
    template <typename TSystem>
    bool HasSystem() const;
    template <typename TSystem>
    TSystem &GetSystem() const;

    // Add and remove entities from their systems
    void AddEntityToSystems(Entity entity); // Checks the component signature of an entity and add the entity to the systems that are interested it
    // Update system so entities with removed components are removed from system
    void UpdateEntityInSystems(Entity entity);
    void RemoveEntityFromSystems(Entity entity);
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&...args)
{
    auto newSystem = std::make_unique<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), std::move(newSystem)));
}

template <typename TSystem>
void Registry::RemoveSystem()
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const
{
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem &Registry::GetSystem() const
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *static_cast<TSystem *>(system->second.get());
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args)
{
    const size_t componentID = Component<TComponent>::GetID();
    const size_t entityID = entity.GetId();

    if (componentID >= componentPools.size())
    {
        componentPools.resize(componentID + 1);
    }

    if (!componentPools[componentID])
    {
        auto newComponentPool = std::make_unique<Pool<TComponent>>();
        componentPools[componentID] = std::move(newComponentPool);
    }

    auto componentPool = static_cast<Pool<TComponent> *>(componentPools[componentID].get());

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityID, newComponent);

    entityComponentSignatures[entityID].set(componentID);

    Logger::Log("Component id = " + std::to_string(componentID) + " was added to entity id " + std::to_string(entityID));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetId();

    // Remove the component from the component list for that entity
    auto componentPool = static_cast<Pool<TComponent> *>(componentPools[componentID].get());
    componentPool->Remove(entityID);

    // Set the component signature for that entity to false
    entityComponentSignatures[entityID].set(componentID, false);

    Logger::Log("Component id = " + std::to_string(componentID) + " was removed from entity id " + std::to_string(entityID));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetId();
    return entityComponentSignatures[entityID].test(componentID);
}

template <typename TComponent>
TComponent &Registry::GetComponent(Entity entity) const
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetId();

    auto componentPool = static_cast<Pool<TComponent> *>(componentPools[componentID].get());

    return componentPool->Get(entityID);
}

template <typename TComponent, typename... TArgs>
void Entity::AddComponent(TArgs &&...args)
{
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent()
{
    registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const
{
    return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent &Entity::GetComponent() const
{
    return registry->GetComponent<TComponent>(*this);
}