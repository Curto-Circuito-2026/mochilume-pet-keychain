#include "Activities/Menu.h"
#include <DisplayManager.h>


Menu::Menu() 
    : Activity("menu", nullptr) {}
void Menu::setup() {
    int y = 0;
    int squareSize = 60;

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

    UIScreen* screen1 = new UIScreen();
    
    UIElement* square = new UIElement(
        "square", 
        90, 
        y, 
        squareStyle, 
        squareStyle, 
        squareStyle);
    
    screen1->addChild(square);

    UIElement* square2 = new UIElement(
        "square2", 
        90, 
        y, 
        squareStyle, 
        squareStyle, 
        squareStyle);
    
    screen1->addChild(square2);
    _screen->changeScreen(screen1);
}
void Menu::loop() {

    _screen->render();
    
    delay(FRAME_DELAY);
}
void Menu::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}