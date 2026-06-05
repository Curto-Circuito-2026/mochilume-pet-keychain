#include <DisplayManager.h>
#include "UI/ScreenManager.h"
#include <vector>
#include <InputManager.h>

ScreenManager* ScreenManager::_instance = nullptr;

#define SCREEN_STRIP 30
ScreenManager::ScreenManager() {
    this->curScreen = nullptr;
    this->dirty = true;
   
    this->canvas = new GFXcanvas16(SCREEN_WIDTH, SCREEN_STRIP);
}
ScreenManager::~ScreenManager() {
}

ScreenManager* ScreenManager::getInstance() {
    if (_instance == nullptr) _instance = new ScreenManager();
    return _instance;
}

void ScreenManager::changeScreen(UIScreen* screen){
    if(curScreen){
        
    }
    Serial.println("troca de tela");
    this->curScreen = screen;
    setDirtyFlag(true);
}

void ScreenManager::setDirtyFlag(bool dirty){
    this->dirty = dirty;
}

void ScreenManager::onButtonPress(uint8_t button){
    if(this->curScreen){this->curScreen->onButtonPress(button);}
}


UIScreen* ScreenManager::getCurScreen(){
    return this->curScreen;
}

void ScreenManager::render(){
    for (int i : InputManager::getInstance()->inputQueue){
        this->onButtonPress(i);
        delay(150);
        InputManager::getInstance()->inputQueue.pop_front();
    }

    if(this->dirty){
        Adafruit_GC9A01A* _tft = DisplayManager::getInstance()->getTFT();
        for(int stripOffset = 0; stripOffset < SCREEN_HEIGHT; stripOffset += SCREEN_STRIP){
            canvas->fillScreen(GC9A01A_BLACK);
            if(this->curScreen){ this->curScreen->render(canvas, stripOffset); }
            _tft->drawRGBBitmap(0, stripOffset, canvas->getBuffer(), SCREEN_WIDTH, SCREEN_STRIP);
        }
        this->dirty = false;
    }
}


