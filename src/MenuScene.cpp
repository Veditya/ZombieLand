#include "MenuScene.hpp"
#include "GameEngine.hpp"
#include "Logger.hpp"


void MenuScene::init()
{
    loadActions();
    if (!loadAssets())
    {
        LOG_ERROR("Menu scene could not load a font; menu text will not be visible");
    }
}
void MenuScene::loadActions(){

    eventMap = std::make_unique<EventMap>(game.settings().input);
}

bool MenuScene::loadAssets(){
    return menuFont.loadFromFile(game.settings().menuFontPath);
}

MenuScene::MenuScene(GameEngine& engine, const std::string& _name)
    : Scene(engine, _name), menu(engine.settings().menuOptions)
{
}


void MenuScene::Render()
{
    auto& window = game.renderer().getWindow();
    window.clear(sf::Color(15, 22, 32));

    if (!menuFont.getInfo().family.empty())
    {
        const auto drawCentered = [&window, this](const std::string& value,
                                                 unsigned int size, float y,
                                                 const sf::Color& color)
        {
            sf::Text text(value, menuFont, size);
            text.setFillColor(color);
            const auto bounds = text.getLocalBounds();
            text.setPosition((window.getSize().x - bounds.width) * 0.5f - bounds.left, y);
            window.draw(text);
        };

        drawCentered(game.settings().menuTitle, 54, 180.0f, sf::Color::White);

        if (showingOptions)
        {
            const auto& input = game.settings().input;
            drawCentered(game.settings().optionsTitle, 32, 330.0f,
                         sf::Color(95, 210, 190));
            drawCentered(input.moveUpLabel + " / " + input.moveDownLabel +
                         "  -  Navigate", 22, 410.0f, sf::Color::White);
            drawCentered(input.selectLabel + "  -  Select", 22, 455.0f,
                         sf::Color::White);
            drawCentered(input.backLabel + "  -  Back", 22, 500.0f,
                         sf::Color::White);
        }
        else
        {
            for (std::size_t i = 0; i < menu.size(); ++i)
            {
                const auto color = i == static_cast<std::size_t>(menuIndex)
                    ? sf::Color(95, 210, 190)
                    : sf::Color(225, 230, 235);
                drawCentered(menu[i], 32, 350.0f + static_cast<float>(i) * 70.0f, color);
            }
            const auto& input = game.settings().input;
            drawCentered(input.moveUpLabel + " / " + input.moveDownLabel +
                         "  -  Navigate     " + input.selectLabel + "  -  Select",
                         16, 680.0f, sf::Color(145, 155, 170));
        }
    }

    game.renderer().updateFramerate();
    window.display();
}

void MenuScene::Input()
{
    auto& window = game.renderer().getWindow();
    sf::Event event;
    while(window.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (showingOptions)
            {
                if (event.key.code == game.settings().input.back ||
                    event.key.code == game.settings().input.select)
                {
                    showingOptions = false;
                }
                continue;
            }

            if (event.key.code == game.settings().input.moveUp && !menu.empty())
            {
                menuIndex = menuIndex == 0
                    ? static_cast<int>(menu.size()) - 1
                    : menuIndex - 1;
                continue;
            }
            if (event.key.code == game.settings().input.moveDown && !menu.empty())
            {
                menuIndex = (menuIndex + 1) % static_cast<int>(menu.size());
                continue;
            }
            if (event.key.code == game.settings().input.select)
            {
                selectCurrentOption();
                continue;
            }
        }
        eventMap->perform(event, game);
    }
}

void MenuScene::selectCurrentOption()
{
    if (menuIndex < 0 || static_cast<std::size_t>(menuIndex) >= menu.size())
    {
        return;
    }

    const auto& selection = menu[static_cast<std::size_t>(menuIndex)];
    if (selection == game.settings().startMenuOption)
    {
        LOG_INFO("Starting play scene from menu");
        game.setCurrentScene(game.settings().playSceneName);
    }
    else if (selection == game.settings().optionsMenuOption)
    {
        showingOptions = true;
    }
    else if (selection == game.settings().exitMenuOption)
    {
        LOG_INFO("Exiting from menu");
        game.renderer().closeWindow();
    }
}
