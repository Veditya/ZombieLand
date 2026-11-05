#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "Scene.hpp"
#include <map>
#include <string>
#include <memory>
#include <functional>

class SceneManager{
public:
    using SceneFactory = std::function<std::shared_ptr<Scene>()>;

private:
    std::shared_ptr<Scene> currentScene;
    std::map<std::string, SceneFactory> sceneFactories;

public:

    bool registerScene(const std::string& sceneName, SceneFactory factory);
    std::shared_ptr<Scene> getCurrentScene();
    void endCurrentScene();
    bool setCurrentScene(const std::string& sceneName);
    ~SceneManager() = default;
};

#endif