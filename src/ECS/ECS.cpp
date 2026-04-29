#include "ECS/ECS.h"
#include <algorithm>
#include "Logger.h"
#include <ranges>

void Entity::Kill() const
{
    registry->KillEntity(*this);
}

void Entity::Tag(const std::string &tag)
{
    registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string &tag) const
{
    return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string &group)
{
    registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string &group) const
{
    return registry->EntityBelongsToGroup(*this, group);
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    // Maybe replace with swap and pop (O(1) instead of O(n))
    std::erase_if(entities, [&entity](const Entity &other)
                  { return entity == other; });
}

const std::vector<Entity> &System::GetSystemEntities() const
{
    return entities;
}

const Signature &System::GetComponenetSignature() const
{
    return componentSignature;
}

Entity Registry::CreateEntity()
{
    size_t entityID;

    if (freeIds.empty())
    {
        entityID = numEntities++;

        // Make sure the entityComponenetSignatures vector can accomodate the new entity
        if (entityID >= entityComponentSignatures.size())
        {
            entityComponentSignatures.resize(entityID + 1);
        }
    }
    else
    {
        // Reuse an id from the list of previously removed entities
        entityID = freeIds.back();
        freeIds.pop_back();
    }

    Entity entity(entityID);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string(entityID));

    return entity;
}

void Registry::KillEntity(Entity entity)
{
    entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity)
{
    // Get the unique ID of the entity
    const auto entityID = entity.GetId();

    // Retrieve the component signature (bitmask) of this entity
    // This signature indicates which components the entity currently has
    const auto &entityComponentSignature = entityComponentSignatures[entityID];

    // Iterate over all systems in the registry
    // std::views::values gives only the system objects, ignoring keys
    for (auto &system : systems | std::views::values)
    {
        // Get the component signature that this system is interested in
        const auto &systemComponentSignature = system->GetComponenetSignature();

        // Check if the entity has all components required by the system
        // '&' performs a bitwise AND between entity and system signatures
        // If the result matches the system's signature, the entity has all required components
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        // If the entity has all required components, add it to the system
        if (isInterested)
        {
            system->AddEntityToSystem(entity);
        }
    }
}

void Registry::UpdateEntityInSystems(Entity entity)
{
    const auto entityID = entity.GetId();
    const auto &entityComponentSignature = entityComponentSignatures[entityID];

    for (auto &system : systems | std::views::values)
    {
        const auto &systemComponentSignature = system->GetComponenetSignature();
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isInterested)
        {
            system->AddEntityToSystem(entity); // Won't duplicate if already there
        }
        else
        {
            system->RemoveEntityFromSystem(entity); // Remove if no longer matches
        }
    }
}

void Registry::RemoveEntityFromSystems(Entity entity)
{
    for (auto &system : systems | std::views::values)
    {
        system->RemoveEntityFromSystem(entity);
    }
}

void Registry::TagEntity(Entity entity, const std::string &tag)
{
    entityPerTag.emplace(tag, entity);
    tagpPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string &tag) const
{
    if (tagpPerEntity.find(entity.GetId()) == tagpPerEntity.end())
    {
        return false;
    }
    return entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string &tag) const
{
    return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity)
{
    auto taggedEntity = tagpPerEntity.find(entity.GetId());
    if (taggedEntity != tagpPerEntity.end())
    {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagpPerEntity.erase(taggedEntity);
    }
}

void Registry::GroupEntity(Entity entity, const std::string &group)
{
    entitiesPerGroup.try_emplace(group, std::set<Entity>());
    entitiesPerGroup[group].insert(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string &group) const
{
    auto groupIt = entitiesPerGroup.find(group);
    if (groupIt == entitiesPerGroup.end())
        return false; // group doesn't exist

    const auto &groupEntities = groupIt->second;
    return groupEntities.find(entity) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string &group) const
{
    auto it = entitiesPerGroup.find(group);
    if (it == entitiesPerGroup.end())
        return {};
    return std::vector<Entity>(it->second.begin(), it->second.end());
}

void Registry::RemoveEntityGroup(Entity entity)
{
    // If in group, remove entity from group management
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity != groupPerEntity.end())
    {
        auto group = entitiesPerGroup.find(groupedEntity->second);
        if (group != entitiesPerGroup.end())
        {
            auto entityInGroup = group->second.find(entity);
            if (entityInGroup != group->second.end())
            {
                group->second.erase(entityInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}

void Registry::Update()
{
    // Processing the entities that are waiting to be created to the active systems
    for (auto &entity : entitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    // Processing the entities that are waiting to be killed from the active systems
    for (auto &entity : entitiesToBeKilled)
    {
        RemoveEntityFromSystems(entity);

        entityComponentSignatures[entity.GetId()].reset();

        // Remove the entity from the component pools
        for (auto &pool : componentPools)
        {
            if (pool)
            {
                pool->RemoveEntityFromPool(entity.GetId());
            }
        }

        // Make the entity id available to be reused
        freeIds.push_back(entity.GetId());

        // Remove any traces of that entity from the tag/group maps
        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
    }
    entitiesToBeKilled.clear();
}