#ifndef ENTITY
#define ENTITY
#include <string>
#include <tuple>
#include <utility>
#include "components.hpp"

class Entity
{
private:
    std::tuple<Transform, Body, BoundingBox, Animation, Object> components;

public:
    int id;
    bool alive = true;
    std::string tag;

public:
    Entity(int _id, const std::string &tg)
        : components(Transform(), Body(), BoundingBox(), Animation(), Object()), id(_id), tag(tg) {}
    template <typename T, typename... TArgs>

    T &add(TArgs &&...mArgs)
    {
        auto &component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }
    template <typename T>
    T &get()
    {
        return std::get<T>(components);
    }

    template <typename T>
    const T &get() const
    {
        return std::get<T>(components);
    }

    template <typename T>
    bool has() const
    {
        return get<T>().exists;
    }
};

#endif