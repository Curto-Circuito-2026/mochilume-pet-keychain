#include "Test.h"
#include <DisplayManager.h>
#include "InputManager.h"
#include "UISystem.h"
#include "ActivityManager.h"
#include "HalConfig.h"
#include <Arduino.h>

void Test::onConfig() {
}

void Test::onPlay() {
}

void Test::onAudioConfig() {
}

void Test::onBack() {
}

Test::Test() 
    : Activity("test", nullptr), mainMenu(nullptr), configMenu(nullptr), lastBtnA(false), lastBtnB(false) {}

void Test::setup() {
    y = 0;
    squareSize = 60;

    UIStyle squareStyle = {
        squareSize, 
        squareSize, 
        0,
        GC9A01A_RED,
        false,
        nullptr,
        1,
        0,0,
        0,0,
        GC9A01A_BLACK,
        1
    };

    UIScreen* screen1 = new UIScreen();
    
    square = new UIElement(
        "square", 
        90, 
        y, 
        squareStyle, 
        squareStyle, 
        squareStyle);
    
    screen1->addChild(square);

    square2 = new UIElement(
        "square2", 
        90, 
        y, 
        squareStyle, 
        squareStyle, 
        squareStyle);
    
    screen1->addChild(square2);
    _screen->changeScreen(screen1);
}

void Test::loop() {
    y += 5.0f; 
    if (y >= SCREEN_HEIGHT + squareSize) y = -squareSize;

    square->setPosition(90, y);

    _screen->render();
    
}

void Test::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}

