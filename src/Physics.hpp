#ifndef PHYSICS
#define PHYSICS
#include <memory>
#include "Entity.hpp"
#include "tmxlite/Object.hpp"

class Physics
{
    public:

    bool collisionDetected(std::shared_ptr<Entity>entity, const tmx::Object& solidObject);
    bool collisionDetected(std::shared_ptr<Entity>entity1, std::shared_ptr<Entity> entity2);

    void collisionResolve(std::shared_ptr<Entity> entity, const tmx::Object& solidObject);
    void collisionResolve(std::shared_ptr<Entity>entity1, std::shared_ptr<Entity> entity2);

    Physics();
    ~Physics();
};

#endif