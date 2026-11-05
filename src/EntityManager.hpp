#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "Entity.hpp"

class EntityManager
{
private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> toAdd;
    std::map<std::string, std::vector<std::shared_ptr<Entity>>> entityMap;
    int nextId = 0;

public:
    EntityManager() = default;
    ~EntityManager() = default;

    std::shared_ptr<Entity> addEntity(const std::string &tg);

    const std::vector<std::shared_ptr<Entity>>& getEntity(const std::string& tag) const;

    std::vector<std::shared_ptr<Entity>> &getEntity();

    std::shared_ptr<Entity> getPlayer(const std::string& playerTag);

    void update();

    void clean();
};

#endif