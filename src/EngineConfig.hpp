#ifndef ENGINE_CONFIG_HPP
#define ENGINE_CONFIG_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>
#include <vector>

#ifndef GAME_ASSET_DIR
#define GAME_ASSET_DIR "assets"
#endif

struct InputBindings
{
    sf::Keyboard::Key moveLeft = sf::Keyboard::Left;
    sf::Keyboard::Key moveRight = sf::Keyboard::Right;
    sf::Keyboard::Key moveUp = sf::Keyboard::Up;
    sf::Keyboard::Key moveDown = sf::Keyboard::Down;
    sf::Keyboard::Key select = sf::Keyboard::Enter;
    sf::Keyboard::Key back = sf::Keyboard::Escape;
    sf::Keyboard::Key toggleCollisionBounds = sf::Keyboard::D;
    sf::Mouse::Button shoot = sf::Mouse::Left;
    std::string moveUpLabel = "UP";
    std::string moveDownLabel = "DOWN";
    std::string selectLabel = "ENTER";
    std::string backLabel = "ESC";
};

struct EngineConfig
{
    unsigned int windowWidth = 1024;
    unsigned int windowHeight = 1024;
    unsigned int frameLimit = 60;
    std::string gameTitle = "Shooting Game";
    float playerSpeed = 5.0f;
    float playerCollisionWidth = 32.0f;
    float playerCollisionHeight = 32.0f;
    unsigned int mapChunkSize = 512;
    float cameraMovementSpeed = 10.0f;
    std::string playerTag = "player";
    std::string menuSceneName = "menu-scene";
    std::string playSceneName = "play-scene";
    std::string tilemapAssetKey = "tilemap";
    std::string glassAssetKey = "glass";
    std::string playerTextureAssetKey = "player-body-stand";
    std::string startMenuOption = "start";
    std::string optionsMenuOption = "options";
    std::string exitMenuOption = "exit";
    std::vector<std::string> menuOptions{
        startMenuOption, optionsMenuOption, exitMenuOption
    };
    std::string assetDirectory = GAME_ASSET_DIR;
    std::string tilemapAsset = "level.tmx";
    std::string glassAsset = "glass.png";
    std::string playerTextureAsset = "robot-stand.png";
    std::string solidObjectProperty = "solid";
    std::string startPointObjectName = "start-point";
    std::string glassMapObjectType = "glass";
    std::string menuFontPath;
    std::string menuTitle = "GAME";
    std::string optionsTitle = "CONTROLS";
    InputBindings input;

    EngineConfig()
    {
#if defined(__APPLE__)
        menuFontPath = "/System/Library/Fonts/Supplemental/Arial.ttf";
#elif defined(_WIN32)
        menuFontPath = "C:/Windows/Fonts/arial.ttf";
#else
        menuFontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#endif
    }

    std::string assetPath(const std::string& filename) const
    {
        return assetDirectory + "/" + filename;
    }
};

#endif
