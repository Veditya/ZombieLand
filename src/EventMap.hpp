
#ifndef EVENT_MAP
#define EVENT_MAP
#include <map>
#include <memory>
#include "ActionMap.hpp"

class EventMap{
    public:
    std::map<sf::Event::EventType, std::unique_ptr<ActionMap>> eventMap;

    explicit EventMap(const InputBindings& bindings)
    {
         eventMap[sf::Event::EventType::Closed] =
             std::make_unique<CloseActionMap>();
         eventMap[sf::Event::EventType::KeyPressed] =
             std::make_unique<KeyPressActionMap>(bindings);
         eventMap[sf::Event::EventType::KeyReleased] =
             std::make_unique<KeyReleaseActionMap>(bindings);
         eventMap[sf::Event::EventType::MouseButtonPressed] =
             std::make_unique<MouseActionMap>(bindings);
    }

    void perform(const sf::Event& event, GameEngine& game)
    {
        auto actionMap = eventMap.find(event.type);
        if(actionMap!=eventMap.end() && actionMap->second)
        {
            actionMap->second->perform(event, game);
        }
    }
};

#endif