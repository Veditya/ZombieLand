#ifndef SCENE
#define SCENE

#include <string>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Asset.hpp"
#include "EventMap.hpp"

class GameEngine;

class Scene{
protected:
    GameEngine& game;

    public:

    const std::string name;
    bool paused = false;
    bool running = true;
    
    public:
    
    std::unique_ptr<EventMap> eventMap;
    std::map<std::string, std::shared_ptr<Asset>> assetMap;
    

    Scene(GameEngine& engine, const std::string& _name):game{engine}, name{_name}{}
    virtual ~Scene() = default;
    
    virtual void init()=0;
    virtual void Input()=0;
    virtual void Update()=0;
    virtual void Render()=0;

};

#endif