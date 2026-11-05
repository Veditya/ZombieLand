#ifndef MAP_OBJECT_BOUNDS
#define MAP_OBJECT_BOUNDS

#include "tmxlite/Object.hpp"

inline tmx::FloatRect getMapObjectBounds(const tmx::Object& object)
{
    auto bounds = object.getAABB();
    if (object.getTileID() != 0)
    {
        const auto& position = object.getPosition();
        bounds.left = position.x;
        bounds.top = position.y - bounds.height;
    }
    return bounds;
}

#endif
