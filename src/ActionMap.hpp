#ifndef ACTION_MAP
#define ACTION_MAP
#include <map>
#include <memory>
#include "Action.hpp"
#include "EngineConfig.hpp"

class GameEngine;

class ActionMap{
   public:
    virtual ~ActionMap() = default;
    virtual void perform(const sf::Event& event, GameEngine& game) = 0;
};

class CloseActionMap final : public ActionMap{
   public:
    void perform(const sf::Event& event, GameEngine& game) override;
};

class KeyPressActionMap : public ActionMap{
   private:
   std::map<sf::Keyboard::Key, std::unique_ptr<Action>> actionMap;
   public:

   explicit KeyPressActionMap(const InputBindings& bindings)
   {
       actionMap[bindings.moveLeft]=std::make_unique<MoveLeft>();
       actionMap[bindings.moveRight]=std::make_unique<MoveRight>();
       actionMap[bindings.moveUp]=std::make_unique<MoveUp>();
       actionMap[bindings.moveDown]=std::make_unique<MoveDown>();
   }

   void perform(const sf::Event& event, GameEngine& game)override
   {
       auto action = actionMap.find(event.key.code);
       if(action!=actionMap.end())
       {
          action->second->perform(event, game);
       }
   }
};

class KeyReleaseActionMap : public ActionMap{
   private:
   std::map<sf::Keyboard::Key, std::unique_ptr<Action>> actionMap;
   public:

   explicit KeyReleaseActionMap(const InputBindings& bindings)
   {
       actionMap[bindings.moveRight] = std::make_unique<StopX>();
       actionMap[bindings.moveLeft] = std::make_unique<StopX>();
       actionMap[bindings.moveDown] = std::make_unique<StopY>();
       actionMap[bindings.moveUp] = std::make_unique<StopY>();
   }
   
   void perform(const sf::Event& event, GameEngine& game)override
   {
       auto action = actionMap.find(event.key.code);
       if(action!=actionMap.end())
       {
          action->second->perform(event, game);
       }
   }
};

class MouseActionMap: public ActionMap
{
   private:
   std::map<sf::Mouse::Button, std::unique_ptr<Action>> actionMap;
   public:

   explicit MouseActionMap(const InputBindings& bindings)
   {
       actionMap[bindings.shoot]=std::make_unique<Shoot>();
   }

   void perform(const sf::Event& event, GameEngine& game)override
   {
       auto action = actionMap.find(event.mouseButton.button);
       if(action!=actionMap.end())
       {
          action->second->perform(event, game);
       }
   }
};

#endif