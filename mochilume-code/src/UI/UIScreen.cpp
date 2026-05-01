#include "UI/UIScreen.h"
#include <HalConfig.h>
#include <UI/ScreenManager.h>



UIScreen::UIScreen(){
    this->selectedElement = nullptr;
}
UIScreen::~UIScreen(){
    for (std::pair<std::string, UIElement *> c : this->elements){delete c.second;}
    this->elements.clear();
    ScreenManager::getInstance()->setDirtyFlag(true);
}

UIElement* UIScreen::getChild(std::string id){return this->elements[id];}

void UIScreen::addChild(UIElement* child){
    if(this->elements[child->id] == nullptr){
        child->setScreen(this);
        this->elements[child->id] = child;
        Serial.print("ADD CHILD");
        ScreenManager::getInstance()->setDirtyFlag(true);
    }
}

void UIScreen::setVisibility(bool visible){
    this->visible = visible;
    ScreenManager::getInstance()->setDirtyFlag(true);

}

void UIScreen::setSelectedElement(UIElement* element){
    this->selectedElement = element;
    ScreenManager::getInstance()->setDirtyFlag(true);
}

void UIScreen::onButtonPress(uint8_t button){
    if(!visible){return;}
    if(selectedElement){selectedElement->onButtonPress(button); return;}
}

void UIScreen::render(Adafruit_GFX* tft, int stripOffset){
    if(!visible){return;}
    for (std::pair<std::string, UIElement *> c : this->elements){c.second->render(tft, stripOffset);}

}