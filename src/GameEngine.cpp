#include "GameEngine.hpp"
#include "Logger.hpp"
#include <utility>

GameEngine::GameEngine(const EngineConfig& engineConfig)
        : config(engineConfig),
          entityManager(),
          renderManager(config),
          physics(),
          sceneManager()
{
    LOG_INFO("Game engine initialized");
}

EntityManager& GameEngine::entities() { return entityManager; }
RenderManager& GameEngine::renderer() { return renderManager; }
Physics& GameEngine::physicsSystem() { return physics; }
const EngineConfig& GameEngine::settings() const { return config; }

bool GameEngine::registerScene(const std::string& name,
                               std::function<std::shared_ptr<Scene>()> factory)
{
    return sceneManager.registerScene(name, std::move(factory));
}

bool GameEngine::setCurrentScene(const std::string& name)
{
    return sceneManager.setCurrentScene(name);
}

void GameEngine::run()
{
    while (renderManager.getWindow().isOpen())
    {
        const auto scene = sceneManager.getCurrentScene();
        if (!scene)
        {
            LOG_ERROR("No active scene; stopping game loop");
            break;
        }

        LOG_INFO("Starting scene '", scene->name, "'");
        scene->init();
        while (renderManager.getWindow().isOpen() &&
               sceneManager.getCurrentScene() == scene && scene->running)
        {
            scene->Input();
            if (!renderManager.getWindow().isOpen() ||
                sceneManager.getCurrentScene() != scene || !scene->running)
            {
                break;
            }

            scene->Update();
            scene->Render();
        }

        if (sceneManager.getCurrentScene() == scene)
        {
            break;
        }
    }
}

void GameEngine::stop()
{
    LOG_INFO("Stopping game engine");
    entityManager.clean();
    renderManager.getWindow().close();
}
