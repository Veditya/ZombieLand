#ifndef ASSET
#define ASSET

#include <SFML/Graphics.hpp>
#include <string>
#include "tmxlite/Map.hpp"

class Asset{
    public:
    std::string src{};

    public:
    virtual bool load(const std::string& source)=0;
    Asset() = default;
    virtual ~Asset() = default;
};

class Text:public Asset{
    public:
    sf::Font font;
    bool load(const std::string& source)override{
        src = source;
        return !src.empty() && font.loadFromFile(source);
    }
};

class TileImage:public Asset{
    public:

    TileImage()=default;
    ~TileImage()=default;
    sf::Texture texture;
    bool load(const std::string& source)override{
        src = source;
        return !src.empty() && texture.loadFromFile(src);
    }

};

class Tilemap:public Asset{
    public:
    
    tmx::Map map;
    public:

    Tilemap()=default;
    ~Tilemap()=default;

    bool load(const std::string& source)override
    {
        src=source;
        return map.load(src);
    }

    

};




#endif