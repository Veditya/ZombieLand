#ifndef COMPONENTS
#define COMPONENTS

#include "Logger.hpp"

#include "SFML/Graphics.hpp"
#include <string>

class Transform{//for kinematics
    public:
    bool exists = false;
    sf::Vector2f position = {0.0, 0.0};
    sf::Vector2f velocity = {0.0, 0.0};
    float angle = 0.0;

public:
    Transform()=default;
    ~Transform() = default;

    void setPosition(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    void setVelocity(float x, float y)
    {
        velocity.x = x;
        velocity.y = y;
    }
    
};

class Body
{// for drawing
    public:
    bool exists = false;
    sf::Sprite sprite;
    sf::Texture texture;
    
    public:

    Body()=default;
    ~Body()=default;

    void set(const std::string& src)
    {
        if (!texture.loadFromFile(src))
        {
            LOG_ERROR("Body could not be set from texture: ", src);
            return;
        }

        sprite.setTexture(texture);

        //set origin from top-left to center
        auto bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width/2.0f,bounds.height/2.0f);
    }

    void setPosition(float x, float y){
        sprite.setPosition(x, y);
    }

    void setOrientation(float angle)
    {
        sprite.setRotation(angle);
    }

    float getOrientation(){
        return sprite.getRotation();
    }

    void rotate(float angularSpeed)
    {
        sprite.rotate(angularSpeed);
    }

};

class Animation{//for animation
    public:
    bool exists = false;
    
};

class BoundingBox{// for collision
    public:
    bool exists = false;
   
    float width = 0.0f;
    float height = 0.0f;

    //position just one frame before collision
    float last_x = 0.0f;
    float last_y = 0.0f;

    BoundingBox() = default;
    BoundingBox(float _width, float _height):
    width{_width},height{_height}
    {

    }

    ~BoundingBox(){

    }
};

class Object{
    private:

    std::string name;
    sf::Vector2f position;
    sf::FloatRect AABB;
    float rotation;
    bool visible;

    public:

    bool exists=false;

    Object()=default;
    ~Object()=default;

    std::string Name(){return name;}
    sf::Vector2f getPosition(){return position;}
    sf::FloatRect getAABB(){return AABB;}
    float Rotation(){return rotation;}
    bool Visible(){return visible;}
    
    void setPosition(sf::Vector2f& _position){
        position = _position;
    }

    void setAABB(sf::FloatRect& aabb){
        AABB = aabb;
    }

    void setRotation(float angle){
       rotation = angle;
    }

    void setVisibility(bool visibility)
    {
        visible = visibility;
    }

    void setName(const std::string& _name)
    {
        name = _name;
    }
};



#endif