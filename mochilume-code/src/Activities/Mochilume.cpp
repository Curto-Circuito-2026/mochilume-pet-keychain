#include "Activities/Mochilume.h"
#include <DisplayManager.h>
#include <ActivityManager.h>


Mochilume::Mochilume() 
    : Activity("mochilume", nullptr) {}
void Mochilume::setup() {
    int squareSize = 20;

    UIStyle squareHoverStyle = {
        squareSize, 
        squareSize, 
        GC9A01A_WHITE,
        nullptr,
        1,
        0,0,
        0,0,
        GC9A01A_BLACK,
        1
    };

    UIStyle squareStyle = {
        squareSize, 
        squareSize, 
        GC9A01A_RED,
        nullptr,
        1,
        0,0,
        0,0,
        GC9A01A_BLACK,
        1
    };
  
    home = new UIScreen();
    stats = new UIScreen();
    battleSelect = new UIScreen();
    battle = new UIScreen();

    this->screens["home"] = home;
    this->screens["stats"] = stats;
    this->screens["battleSelect"] = battleSelect;
    this->screens["battle"] = battle;
    
    //HOME
    UIElement* exitButton = new UIElement(
        "exitButton", 
        10, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);

    UIElement* statsButton = new UIElement(
        "statsButton", 
        50, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);


    UIElement* battleSelectButton = new UIElement(
        "battleSelectButton", 
        90, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);

    exitButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("SAIR - MENU"); ActivityManager::getInstance()->setActivity("menu"); });

    statsButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR STATS"); _screen->changeScreen(stats); });
    statsButton->setAction(BTN_LEFT, [this](UIElement* element) { Serial.println("TROCA PRO BOTAO 2");  element->getScreen()->setSelectedIndex(1); });

    battleSelectButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR BS"); _screen->changeScreen(battleSelect); });
    battleSelectButton->setAction(BTN_RIGHT, [this](UIElement* element) { Serial.println("TROCA PRO BOTAO 1"); element->getScreen()->setSelectedIndex(0); });

    home->addChild(exitButton);
    home->addChild(statsButton);
    home->addChild(battleSelectButton);

    home->setSelectedIndex(0);

    //STATS
    UIElement* square25 = new UIElement(
        "square25", 
        20, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);

    square25->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });
    stats->addChild(square25);
    stats->setSelectedIndex(0);
    
    //BATTLE SELECT
    UIElement* square3 = new UIElement(
        "square3", 
        20, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);
    
    square3->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });
    battleSelect->addChild(square3);
    battleSelect->setSelectedIndex(0);

    //BATTLE
    UIElement* square4 = new UIElement(
        "square4", 
        90, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);
    
    battle->addChild(square4);


    _screen->changeScreen(home);
}
void Mochilume::loop() {
   UIScreen * cs = _screen->getCurScreen();
   if(cs == home){
    //lora checar se chegou convite pra batalhar
   }else if(cs == stats){
    
   }else if(cs == battleSelect){
    //lora buscar gente perto
   }else if(cs == battle){
    //logica de combate -> lora enviar pro oponente
   }

    _screen->render();
    
    delay(FRAME_DELAY);
}
void Mochilume::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}