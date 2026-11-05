#ifndef ACTION
#define ACTION

#include <SFML/Window/Event.hpp>

class GameEngine;
// enum class PlayerAction{
//     LEFT,
//     RIGHT,
//     UP,
//     DOWN,
//     SHOOT,
// };

// enum class MenuAction{
//     ABOVE,
//     BELOW,
//     SELECT
// }; 

class Action{
    public:
    virtual ~Action() = default;
    virtual void perform(const sf::Event& event, GameEngine& game)=0;
};

class MoveLeft:public Action{
    void perform(const sf::Event& event, GameEngine& game)override;
};

class MoveRight:public Action{
    void perform(const sf::Event& event, GameEngine& game)override;
};

class MoveUp:public Action{
    void perform(const sf::Event& event, GameEngine& game)override;
};

class MoveDown:public Action{
    void perform(const sf::Event& event, GameEngine& game)override;
};

class StopX :public Action{
    public:
    void perform(const sf::Event& event, GameEngine& game)override;
};

class StopY :public Action{
    public:
    void perform(const sf::Event& event, GameEngine& game)override;
};

class Shoot:public Action{
    public :
    void perform(const sf::Event& event, GameEngine& game)override;
};

#endif