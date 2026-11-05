#ifndef MENU_SCENE
#define MENU_SCENE

#include "Scene.hpp"
#include <string>
#include <vector>

class MenuScene:public Scene{
    public:

    std::vector<std::string>menu;
    int menuIndex=0;
    sf::Font menuFont;
    bool showingOptions=false;
    
    private:

    void loadActions();
    bool loadAssets();
    

    public:

    MenuScene(GameEngine& game, const std::string& _name);

    void init()override;
    void Input()override;
    void Update()override {}
    void Render()override;

private:
    void selectCurrentOption();
};

#endif