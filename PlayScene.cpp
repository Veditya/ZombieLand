#include "PlayScene.hpp"
#include "GameEngine.hpp"
#include "Logger.hpp"
#include "SFMLOrthogonalLayer.hpp"

void PlayScene::cacheLayers()
{
    const auto tilemap = std::dynamic_pointer_cast<Tilemap>(assetMap["tilemap"]);
    const auto bounds = tilemap->map.getBounds();
    GameEngine::engine()->renderer->setCameraBounds(
            sf::FloatRect(bounds.left, bounds.top, bounds.width, bounds.height));

    tileLayers.clear();
    objectLayers.clear();
    const auto& layers = tilemap->map.getLayers();
    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        if (layers[i]->getType() == tmx::Layer::Type::Tile)
        {
            tileLayers.push_back(std::make_unique<MapLayer>(tilemap->map, i));
        }
        else if (layers[i]->getType() == tmx::Layer::Type::Object)
        {
            objectLayers.push_back(&layers[i]->getLayerAs<tmx::ObjectGroup>());
        }
    }
}

void PlayScene::loadActions(){

    actionMap[sf::Keyboard::Key::Left]=std::make_shared<MoveLeft>();
    actionMap[sf::Keyboard::Key::Right]=std::make_shared<MoveRight>();
    actionMap[sf::Keyboard::Key::Up]  =std::make_shared<MoveUp>();
    actionMap[sf::Keyboard::Key::Down]=std::make_shared<MoveDown>();
    eventMap[sf::Mouse::Button::Left] =std::make_shared<Shoot>();
   
}

bool PlayScene::loadAssets()
{
    assetMap["tilemap"]=std::make_shared<Tilemap>();
    assetMap["glass"]=std::make_shared<TileImage>();
    assetMap["player-body-stand"]=std::make_shared<TileImage>();

    assetMap["tilemap"]->src = "assets/level.tmx";
    assetMap["glass"]->src = "assets/glass.png";
    assetMap["player-body-stand"]->src = "assets/robot-stand.png";

    bool status = true; //asset load status
    for(const auto& asset : assetMap)
    {
        auto& name = asset.first;
        auto& resource = asset.second;
        auto& src = resource->src;
        bool currentStatus = resource->load(src);

        status = status && currentStatus;

        if(!currentStatus)
        {
            LOG_ERROR("failed to load : ", name, " from ", src);
            continue;
        }
        if( name == "tilemap")
        {
            cacheLayers();
        }
    }
    
    return status;
}

PlayScene::PlayScene(const std::string& _name):Scene(_name){}

void PlayScene::init()
{
    loadActions();
    if (!loadAssets())
    {
        LOG_ERROR("Play scene initialization failed because one or more assets did not load");
    }
}
void PlayScene::start()
{
    init();
    while(running)
    {
        spawnPlayer();
        Input();
        if(!paused) 
        {
           //Movement();
           //Rotation();
           //Collision();
           //Enemies();
        }
        Update();
        Render();
    }
}


void PlayScene::spawnPlayer(){

    //get old player instance
    auto entity_manager = GameEngine::engine()->entity_manager;
    auto oldPlayer = entity_manager->getPlayer();
    if(!oldPlayer)
    {
        //add new player instance
        auto newPlayer = entity_manager->addEntity("player"); 
        auto& transform = newPlayer->add<Transform>();
        auto& body = newPlayer->add<Body>();
        body.set(assetMap["player-body-stand"]->src);

        //set start point location of player
        const auto& objects = objectLayers[0]->getObjects();
        for(const auto& object : objects)
        {
            const std::string objectType = object.getType();
            if (object.visible() && objectType=="start-point")
            {
                const auto& location = object.getPosition();
                transform.setPosition(location.x, location.y);
            }
        }
        
    }
}

void PlayScene::Update()
{
    GameEngine::engine()->entity_manager->update();
}

void PlayScene::Input()
{
    auto& window = GameEngine::engine()->renderer->getWindow();
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:end();break;
            case sf::Event::KeyPressed:actionMap[event.key.code]->perform();break;
            case sf::Event::MouseButtonPressed:eventMap[event.mouseButton.button]->perform(event);break;
            default:break;
        }
    }
}

void PlayScene::Render()
{
    //get the stuff to render
    auto& world = std::dynamic_pointer_cast<Tilemap>(assetMap["tilemap"])->map;
    auto& window = GameEngine::engine()->renderer->getWindow();
    auto& camera = GameEngine::engine()->renderer->getCamera();
    auto& entities = GameEngine::engine()->entity_manager->getEntity();
    const auto& glassTile = std::dynamic_pointer_cast<TileImage>(assetMap["glass"]);

    window.clear(sf::Color::Black);
    window.setView(camera);
    
    //1. render the world 
    for(const auto& layer : tileLayers)//rendering tiles
    {
       window.draw(*layer);
    }

    for(const auto* layer : objectLayers)//rendering objects
    {
        const auto& objects = layer->getObjects();
        for(const auto& object : objects)
        {
            const std::string objectType = object.getType();
            if (object.visible() && objectType=="glass")
            {
                if(!glassTile)
                {
                    LOG_ERROR("Failed to get image " + objectType, assetMap["glass"]->src, "'");
                    continue;
                }
                sf::Sprite sprite(glassTile->texture);
                const auto& pos = object.getPosition();
                const auto& tileSize = world.getTileSize();
                sprite.setPosition(pos.x, pos.y - (float)tileSize.x);
                window.draw(sprite);
            }
        }
    }

    //2. render the entities
    for(const auto& entity : entities)
    {
        if(entity && entity->alive && entity->has<Transform>())
        {
            auto& currPosition = entity->get<Transform>().position;
            auto& body = entity->get<Body>();
            body.setPosition(currPosition.x , currPosition.y);
            window.draw(body.sprite);
        }
    }

    //framerate updation
    GameEngine::engine()->renderer->updateFramerate();
    window.display();
}
void PlayScene::Movement(){
    //camera movement is not handled here
    
}
void PlayScene::Rotation(){

}
void PlayScene::Collision(){

}
void PlayScene::Enemies(){

}
    
PlayScene::~PlayScene(){

}