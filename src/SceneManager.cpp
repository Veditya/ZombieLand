#include "SceneManager.hpp"
#include "Logger.hpp"
#include <utility>

std::shared_ptr<Scene> SceneManager::getCurrentScene()
{
    return currentScene;
}

bool SceneManager::registerScene(const std::string& sceneName, SceneFactory factory)
{
    if (sceneName.empty() || !factory)
    {
        LOG_ERROR("Cannot register a scene without a name and factory");
        return false;
    }

    sceneFactories[sceneName] = std::move(factory);
    return true;
}

void SceneManager::endCurrentScene(){
    if(currentScene)
    {
        currentScene->running=false;
    }
}

bool SceneManager::setCurrentScene(const std::string& sceneName)
{
    const auto factory = sceneFactories.find(sceneName);
    if (factory == sceneFactories.end())
    {
        LOG_WARNING("Unknown scene '", sceneName, "'; keeping current scene");
        return false;
    }

    auto scene = factory->second();
    if (!scene)
    {
        LOG_ERROR("Scene factory for '", sceneName, "' returned no scene");
        return false;
    }

    const std::string previousScene = currentScene ? currentScene->name : "<none>";
    if (currentScene)
    {
        currentScene->running = false;
    }
    currentScene = std::move(scene);
    LOG_INFO("Scene changed from '", previousScene, "' to '", sceneName, "'");
    return true;
}