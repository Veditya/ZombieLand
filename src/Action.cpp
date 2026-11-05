#include "Action.hpp"
#include "ActionMap.hpp"
#include "GameEngine.hpp"

void CloseActionMap::perform(const sf::Event&, GameEngine& game)
{
    game.renderer().closeWindow();
}

void MoveLeft::perform(const sf::Event&, GameEngine& game)
{
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if (player && player->alive && player->has<Transform>())
    {
        player->get<Transform>().velocity.x = -game.settings().playerSpeed;
    }
}

void MoveRight::perform(const sf::Event&, GameEngine& game)
{
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if (player && player->alive && player->has<Transform>())
    {
        player->get<Transform>().velocity.x = game.settings().playerSpeed;
    }
}

void MoveUp::perform(const sf::Event&, GameEngine& game)
{
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if (player && player->alive && player->has<Transform>())
    {
        player->get<Transform>().velocity.y = -game.settings().playerSpeed;
    }
}

void MoveDown::perform(const sf::Event&, GameEngine& game)
{
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if (player && player->alive && player->has<Transform>())
    {
        player->get<Transform>().velocity.y = game.settings().playerSpeed;
    }
}

void StopX::perform(const sf::Event&, GameEngine& game)
{
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if (player && player->alive && player->has<Transform>())
    {
        player->get<Transform>().velocity.x = 0.0f;
    }
}

void StopY::perform(const sf::Event&, GameEngine& game)
{
    auto player = game.entities().getPlayer(game.settings().playerTag);
    if (player && player->alive && player->has<Transform>())
    {
        player->get<Transform>().velocity.y = 0.0f;
    }
}

void Shoot::perform(const sf::Event&, GameEngine&)
{
}
