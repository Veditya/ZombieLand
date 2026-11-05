#include "Physics.hpp"
#include "MapObjectBounds.hpp"
#include <algorithm>

Physics::Physics(){

}

Physics::~Physics(){

}

bool Physics::collisionDetected(std::shared_ptr<Entity>entity, const tmx::Object& solidObject){
    const auto& bounds = entity->get<BoundingBox>();
    const auto& position = entity->get<Transform>().position;
    const auto objectBounds = getMapObjectBounds(solidObject);

    const float entityLeft = position.x - bounds.width / 2.0f;
    const float entityTop = position.y - bounds.height / 2.0f;
    return entityLeft < objectBounds.left + objectBounds.width &&
           entityLeft + bounds.width > objectBounds.left &&
           entityTop < objectBounds.top + objectBounds.height &&
           entityTop + bounds.height > objectBounds.top;
}

void Physics::collisionResolve(std::shared_ptr<Entity> entity, const tmx::Object& solidObject)
{
    auto& bounds = entity->get<BoundingBox>();
    auto& position = entity->get<Transform>().position;
    const auto objectBounds = getMapObjectBounds(solidObject);

    const float halfWidth = bounds.width / 2.0f;
    const float halfHeight = bounds.height / 2.0f;
    const float overlapX = std::min(position.x + halfWidth, objectBounds.left + objectBounds.width) -
                           std::max(position.x - halfWidth, objectBounds.left);
    const float overlapY = std::min(position.y + halfHeight, objectBounds.top + objectBounds.height) -
                           std::max(position.y - halfHeight, objectBounds.top);
    if (overlapX <= 0.0f || overlapY <= 0.0f)
    {
        return;
    }

    const float previousLeft = bounds.last_x - halfWidth;
    const float previousRight = bounds.last_x + halfWidth;
    const float previousTop = bounds.last_y - halfHeight;
    const float previousBottom = bounds.last_y + halfHeight;

    if (previousRight <= objectBounds.left)
    {
        position.x = objectBounds.left - halfWidth;
    }
    else if (previousLeft >= objectBounds.left + objectBounds.width)
    {
        position.x = objectBounds.left + objectBounds.width + halfWidth;
    }
    else if (previousBottom <= objectBounds.top)
    {
        position.y = objectBounds.top - halfHeight;
    }
    else if (previousTop >= objectBounds.top + objectBounds.height)
    {
        position.y = objectBounds.top + objectBounds.height + halfHeight;
    }
    else if (overlapX < overlapY)
    {
        position.x = position.x < objectBounds.left + objectBounds.width / 2.0f
            ? objectBounds.left - halfWidth
            : objectBounds.left + objectBounds.width + halfWidth;
    }
    else
    {
        position.y = position.y < objectBounds.top + objectBounds.height / 2.0f
            ? objectBounds.top - halfHeight
            : objectBounds.top + objectBounds.height + halfHeight;
    }
}

bool Physics::collisionDetected(std::shared_ptr<Entity>entity1, std::shared_ptr<Entity>entity2)
{
    const auto& position1 = entity1->get<Transform>().position;
    const auto& position2 = entity2->get<Transform>().position;
    const auto& bounds1 = entity1->get<BoundingBox>();
    const auto& bounds2 = entity2->get<BoundingBox>();

    const float left1 = position1.x - bounds1.width / 2.0f;
    const float top1 = position1.y - bounds1.height / 2.0f;
    const float left2 = position2.x - bounds2.width / 2.0f;
    const float top2 = position2.y - bounds2.height / 2.0f;
    return left1 < left2 + bounds2.width &&
           left1 + bounds1.width > left2 &&
           top1 < top2 + bounds2.height &&
           top1 + bounds1.height > top2;
}

void Physics::collisionResolve(std::shared_ptr<Entity>entity1, std::shared_ptr<Entity>entity2)
{

}
