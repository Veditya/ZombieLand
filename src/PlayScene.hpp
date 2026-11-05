#ifndef PLAY_SCENE
#define PLAY_SCENE
#include "Scene.hpp"
#include "EventMap.hpp"

#include <memory>
#include <vector>

class MapLayer;
namespace tmx { class ObjectGroup; }

class PlayScene:public Scene{
    private:

    std::vector<std::unique_ptr<MapLayer>> tileLayers;
    std::vector<const tmx::ObjectGroup*> objectLayers;

    bool showCollisionBounds = false;
    bool collisionBoundsKeyDown = false;

    void loadActions();
    bool loadAssets();

    public:

    PlayScene(GameEngine& game, const std::string& _name);

    void init()override;
    void spawnPlayer();
    void cacheLayers();
    void Update()override;
    void Input()override;
    void Render()override;
    void Movement();
    void Rotation();
    void Collision();
    
    ~PlayScene() override;
};

#endif