#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "SceneManager.hpp"
#include "EntityManager.hpp"
#include "RenderManager.hpp"
#include "Physics.hpp"
#include "EngineConfig.hpp"

#include <functional>
#include <memory>
#include <string>

class GameEngine{
private:
   EngineConfig config;
   EntityManager entityManager;
   RenderManager renderManager;
   Physics physics;
   SceneManager sceneManager;

public:
   explicit GameEngine(const EngineConfig& engineConfig = EngineConfig{});
   EntityManager& entities();
   RenderManager& renderer();
   Physics& physicsSystem();
   const EngineConfig& settings() const;
   bool registerScene(const std::string& name, std::function<std::shared_ptr<Scene>()> factory);
   bool setCurrentScene(const std::string& name);
   void run();
   void stop();
   ~GameEngine() = default;
};

#endif