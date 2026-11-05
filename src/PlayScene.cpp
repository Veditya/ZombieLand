#include "PlayScene.hpp"
#include "GameEngine.hpp"
#include "Logger.hpp"
#include "MapObjectBounds.hpp"
#include "SFMLOrthogonalLayer.hpp"
#include <cmath>

namespace
{
    bool isSolidObject(const tmx::Object& object, const std::string& propertyName)
    {
        for (const auto& property : object.getProperties())
        {
            if (property.getName() == propertyName &&
                property.getType() == tmx::Property::Type::Boolean &&
                property.getBoolValue())
            {
                return true;
            }
        }
        return false;
    }
}

void PlayScene::cacheLayers()
{
    const auto asset = assetMap.find(game.settings().tilemapAssetKey);
    const auto tilemap = asset == assetMap.end()
        ? nullptr
        : std::dynamic_pointer_cast<Tilemap>(asset->second);
    if (!tilemap)
    {
        LOG_ERROR("Cannot cache map layers without a loaded tilemap");
        return;
    }
    const auto bounds = tilemap->map.getBounds();
    game.renderer().setCameraBounds(
            sf::FloatRect(bounds.left, bounds.top, bounds.width, bounds.height));

    tileLayers.clear();           
    objectLayers.clear();
    const auto& layers = tilemap->map.getLayers();
    for (std::size_t i = 0; i < layers.size(); ++i)
    {
        if (layers[i]->getType() == tmx::Layer::Type::Tile)
        {
            tileLayers.push_back(std::make_unique<MapLayer>(
                tilemap->map, i, game.settings().mapChunkSize));
        }
        else if (layers[i]->getType() == tmx::Layer::Type::Object)
        {
            objectLayers.push_back(&layers[i]->getLayerAs<tmx::ObjectGroup>());
        }
    }
}

void PlayScene::loadActions()
{
    eventMap = std::make_unique<EventMap>(game.settings().input);
}

bool PlayScene::loadAssets()
{
    const auto& settings = game.settings();
    assetMap[settings.tilemapAssetKey] = std::make_shared<Tilemap>();
    assetMap[settings.glassAssetKey] = std::make_shared<TileImage>();
    assetMap[settings.playerTextureAssetKey] = std::make_shared<TileImage>();

    assetMap[settings.tilemapAssetKey]->src = settings.assetPath(settings.tilemapAsset);
    assetMap[settings.glassAssetKey]->src = settings.assetPath(settings.glassAsset);
    assetMap[settings.playerTextureAssetKey]->src =
        settings.assetPath(settings.playerTextureAsset);

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
        if (name == settings.tilemapAssetKey)
        {
            cacheLayers();
        }
    }
    
    return status;
}

PlayScene::PlayScene(GameEngine& engine, const std::string& _name)
    : Scene(engine, _name)
{
}

void PlayScene::init()
{
    loadActions();
    if (!loadAssets())
    {
        LOG_ERROR("Play scene initialization failed because one or more assets did not load");
    }
}
void PlayScene::spawnPlayer()
{
    //get old player instance
    auto& entityManager = game.entities();
    auto oldPlayer = entityManager.getPlayer(game.settings().playerTag);
    if(!oldPlayer)
    {
        //add new player instance
        auto newPlayer = entityManager.addEntity(game.settings().playerTag);
        auto& transform = newPlayer->add<Transform>(); transform.setVelocity(0.0f, 0.0f); // Initialize velocity to zero
        auto& body = newPlayer->add<Body>();
        body.set(assetMap.at(game.settings().playerTextureAssetKey)->src);
        newPlayer->add<BoundingBox>(game.settings().playerCollisionWidth,
                                    game.settings().playerCollisionHeight);
        
        //set start point location of player
        for (const auto* layer : objectLayers)
        {
            for (const auto& object : layer->getObjects())
            {
                if (object.visible() &&
                    object.getName() == game.settings().startPointObjectName)
                {
                    const auto& location = object.getPosition();
                    transform.setPosition(location.x, location.y);
                    LOG_INFO("player spawned at : ", location.x, ", ", location.y);
                    return;
                }
            }
        }
        
    }
}

void PlayScene::Update()
{
    if (!paused)
    {
        Movement();
        Rotation();
        Collision();
    }

    game.entities().update();

    //set current position of player as last bounding box position (will be used for collision resolution)
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if(player && player->alive && player->has<Transform>() && player->has<BoundingBox>())
    {
        auto& playerBounds = player->get<BoundingBox>();
        auto& transform = player->get<Transform>();

        playerBounds.last_x = transform.position.x;
        playerBounds.last_y = transform.position.y;
    }
}

void PlayScene::Input()
{
    spawnPlayer();
    auto& window = game.renderer().getWindow();
    sf::Event event;
    while(window.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == game.settings().input.toggleCollisionBounds)
        {
            if (!collisionBoundsKeyDown)
            {
                showCollisionBounds = !showCollisionBounds;
                collisionBoundsKeyDown = true;
            }
            continue;
        }
        if (event.type == sf::Event::KeyReleased &&
            event.key.code == game.settings().input.toggleCollisionBounds)
        {
            collisionBoundsKeyDown = false;
        }
        if (event.type == sf::Event::LostFocus)
        {
            collisionBoundsKeyDown = false;
        }
        eventMap->perform(event, game);
    }
}

void PlayScene::Render()
{
    //get the stuff to render
    auto& window = game.renderer().getWindow();
    auto& camera = game.renderer().getCamera();
    auto& entities = game.entities().getEntity();
    const auto glassAsset = assetMap.find(game.settings().glassAssetKey);
    const auto glassTile = glassAsset == assetMap.end()
        ? nullptr
        : std::dynamic_pointer_cast<TileImage>(glassAsset->second);

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
            if (object.visible() &&
                objectType == game.settings().glassMapObjectType)
            {
                if(!glassTile)
                {
                    LOG_ERROR("Failed to resolve map image type '", objectType,
                              "' in the play scene assets");
                    continue;
                }
                sf::Sprite sprite(glassTile->texture);
                const auto bounds = getMapObjectBounds(object);
                sprite.setPosition(bounds.left, bounds.top);
                window.draw(sprite);
            }
        }
    }

    //2. render the entities
    for(const auto& entity : entities)
    {
        if(entity && entity->alive && entity->has<Body>() &&
           entity->has<Transform>())
        {
            auto& currPosition = entity->get<Transform>().position;
            auto& body = entity->get<Body>();
            body.setPosition(currPosition.x , currPosition.y);
            window.draw(body.sprite);
        }
    }

    if (showCollisionBounds)
    {
        sf::RectangleShape outline;
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(2.0f);

        outline.setOutlineColor(sf::Color(255, 80, 80));
        for (const auto* layer : objectLayers)
        {
            for (const auto& object : layer->getObjects())
            {
                if (!isSolidObject(object, game.settings().solidObjectProperty))
                {
                    continue;
                }

                const auto bounds = getMapObjectBounds(object);
                outline.setPosition(bounds.left, bounds.top);
                outline.setSize(sf::Vector2f(bounds.width, bounds.height));
                window.draw(outline);
            }
        }

        const auto player = game.entities().getPlayer(game.settings().playerTag);
        if (player && player->alive && player->has<Transform>() &&
            player->has<BoundingBox>())
        {
            const auto& position = player->get<Transform>().position;
            const auto& bounds = player->get<BoundingBox>();
            outline.setOutlineColor(sf::Color(80, 220, 255));
            outline.setPosition(position.x - bounds.width / 2.0f,
                                position.y - bounds.height / 2.0f);
            outline.setSize(sf::Vector2f(bounds.width, bounds.height));
            window.draw(outline);
        }
    }

    //framerate updation
    game.renderer().updateFramerate();
    window.display();
}
void PlayScene::Movement(){

    //player and enemies movement is handled here
    auto& entityManager = game.entities();
    auto& entities = entityManager.getEntity();
    for(auto entity : entities)
    {
        if(entity && entity->alive && entity->has<Transform>())
        {
            auto& transform = entity->get<Transform>();
            transform.position.x += transform.velocity.x;
            transform.position.y += transform.velocity.y;
        }
    }

    //camera movement is handled here
    auto& renderer = game.renderer();
    auto& camera = renderer.getCamera();
    auto player = entityManager.getPlayer(game.settings().playerTag);
    if(player && player->alive && player->has<Transform>())
    {
        const auto& transform = player->get<Transform>();
        const auto& position = transform.position;
        const auto& velocity = transform.velocity;
        camera.setCenter(position.x,position.y);
        renderer.moveCamera(velocity.x, velocity.y);

    }
    
}
void PlayScene::Rotation()
{
    //player's orientation towards mouse is handled here
    auto& renderer = game.renderer();
    auto& window = renderer.getWindow();
    auto player = game.entities().getPlayer(game.settings().playerTag);

    if(player && player->alive && player->has<Transform>() && player->has<Body>())
    {
        auto& transform = player->get<Transform>();
        const auto mousePixel = sf::Mouse::getPosition(window);
        const auto mouseWorld = window.mapPixelToCoords(mousePixel, renderer.getCamera());
        const auto direction = mouseWorld - transform.position;
        if (direction.x == 0.0f && direction.y == 0.0f)
        {
            return;
        }

        constexpr float radiansToDegrees = 57.2957795131f;
        transform.angle = std::atan2(direction.y, direction.x) * radiansToDegrees;
        auto& body = player->get<Body>();
        body.setOrientation(transform.angle);
    }
}
void PlayScene::Collision(){
    //collision of entities with objects having solid property
    auto& physics = game.physicsSystem();
    auto& entities = game.entities().getEntity();

    for(const auto* layer : objectLayers)// pick out objects
    {
        const auto& objects = layer->getObjects();
        for( auto& object : objects)
        {
            if (isSolidObject(object, game.settings().solidObjectProperty))
            {
                for(auto entity : entities)
                {
                    //pick out bounding boxed entity
                    if(entity && entity->alive && entity->has<BoundingBox>())
                    {
                        //check collision 
                        if(physics.collisionDetected(entity, object))
                        {
                            //resolve collision
                            physics.collisionResolve(entity, object);
                        }
                    }
                }
            }
        }
    }

    //collision between player and enemies
    auto player = game.entities().getPlayer(game.settings().playerTag);
    for(auto entity : entities)
    {
        if(entity && entity->alive &&
           entity->tag != game.settings().playerTag && entity->has<BoundingBox>())
        {
            if (player && physics.collisionDetected(player, entity))
            {
                physics.collisionResolve(player, entity);
            }
        }
    }
    

}
PlayScene::~PlayScene() = default;