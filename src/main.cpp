#include "GameEngine.hpp"
#include "Logger.hpp"
#include "MenuScene.hpp"
#include "PlayScene.hpp"

#include <iostream>
#include <memory>

int main()
{
    if (!Logger::initialize())
    {
        std::cerr << "Failed to open log.txt for writing\n";
    }

    GameEngine engine;
    const auto& settings = engine.settings();
    const bool scenesRegistered =
        engine.registerScene(settings.menuSceneName, [&engine]
        {
            return std::make_shared<MenuScene>(
                engine, engine.settings().menuSceneName);
        }) &&
        engine.registerScene(settings.playSceneName, [&engine]
        {
            return std::make_shared<PlayScene>(
                engine, engine.settings().playSceneName);
        });

    if (!scenesRegistered || !engine.setCurrentScene(settings.menuSceneName))
    {
        LOG_ERROR("Failed to configure the initial game scenes");
        return 1;
    }

    engine.run();
    engine.stop();
    return 0;
}
