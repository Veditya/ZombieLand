#include "EntityManager.hpp"
#include <algorithm>

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &_tag)
{
    auto entity = std::make_shared<Entity>(nextId++, _tag);
    toAdd.push_back(entity);
    return entity;
}

std::vector<std::shared_ptr<Entity>> &EntityManager::getEntity()
{
    return entities;
}
const std::vector<std::shared_ptr<Entity>>& EntityManager::getEntity(const std::string& tag) const
{
    static const std::vector<std::shared_ptr<Entity>> empty;
    const auto entitiesForTag = entityMap.find(tag);
    return entitiesForTag == entityMap.end() ? empty : entitiesForTag->second;
}

std::shared_ptr<Entity> EntityManager::getPlayer(const std::string& playerTag)
{
    const auto playerEntities = entityMap.find(playerTag);
    if (playerEntities == entityMap.end() || playerEntities->second.empty())
    {
        return nullptr;
    }

    return playerEntities->second.front();
}

void EntityManager::update()
{
    // Add new entities
    for (auto &entity : toAdd)
    {
        entities.push_back(entity);
        entityMap[entity->tag].push_back(entity);
    }
    toAdd.clear();

    // Remove inactive entities
    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [](const std::shared_ptr<Entity> &entity)
                                  { return !entity->alive; }),
                   entities.end());

    // Update entity maps
    for (auto &pair : entityMap)
    {
        pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(),
                                         [](const std::shared_ptr<Entity> &entity)
                                         { return !entity->alive; }),
                          pair.second.end());
    }
}

void EntityManager::clean()
{
    toAdd.clear();
    entities.clear();
    entityMap.clear();
    nextId = 0;
}