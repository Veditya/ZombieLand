#ifndef RENDER_MANAGER
#define RENDER_MANAGER

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>
#include "EngineConfig.hpp"

class RenderManager{
    private:

    sf::RenderWindow window;
    sf::View camera;
    sf::FloatRect cameraBounds;
    std::string gameTitle;
    float cameraMovementSpeed;
    sf::Clock fpsClock;
    unsigned int fpsFrameCount = 0;

    public:

    explicit RenderManager(const EngineConfig& config)
    :window{sf::VideoMode(config.windowWidth, config.windowHeight), config.gameTitle}
    ,camera{sf::View(sf::FloatRect(0.0f, 0.0f,
                                   static_cast<float>(config.windowWidth),
                                   static_cast<float>(config.windowHeight)))}
    ,cameraBounds{0.f, 0.f, static_cast<float>(config.windowWidth),
                  static_cast<float>(config.windowHeight)}
    ,gameTitle{config.gameTitle}
    ,cameraMovementSpeed{config.cameraMovementSpeed}
    {
        window.setFramerateLimit(config.frameLimit);
    }

    sf::RenderWindow& getWindow()
    {
        return window;
    }

    void closeWindow()
    {
        window.close();
    }

    sf::View& getCamera()
    {
        return camera;
    }

    void updateFramerate()
    {
        ++fpsFrameCount;
        const float elapsedSeconds = fpsClock.getElapsedTime().asSeconds();
        if (elapsedSeconds >= 1.f)
        {
            const auto fps = static_cast<unsigned int>(fpsFrameCount / elapsedSeconds);
            window.setTitle(gameTitle + " | FPS: " + std::to_string(fps));
            fpsFrameCount = 0;
            fpsClock.restart();
        }
    }

    void setCameraBounds(const sf::FloatRect& bounds)
    {
        cameraBounds = bounds;
        moveCamera(0.f, 0.f);
    }

    float getCameraSpeed(){
        return cameraMovementSpeed;
    }

    void setCameraSpeed(float speed){
        cameraMovementSpeed = speed;
    }

    void moveCamera(float offsetX,float offsetY)
    {
        const auto center = camera.getCenter();
        const auto viewSize = camera.getSize();
        const float minX = cameraBounds.left + viewSize.x / 2.f;
        const float maxX = cameraBounds.left + cameraBounds.width - viewSize.x / 2.f;
        const float minY = cameraBounds.top + viewSize.y / 2.f;
        const float maxY = cameraBounds.top + cameraBounds.height - viewSize.y / 2.f;
        const float centerX = minX <= maxX
            ? std::max(minX, std::min(center.x + offsetX, maxX))
            : cameraBounds.left + cameraBounds.width / 2.f;
        const float centerY = minY <= maxY
            ? std::max(minY, std::min(center.y + offsetY, maxY))
            : cameraBounds.top + cameraBounds.height / 2.f;

        camera.setCenter(centerX, centerY);
    }

    ~RenderManager() = default;
};

#endif