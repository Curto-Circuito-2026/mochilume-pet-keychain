#include "UI/UIScreen.h"
#include <HalConfig.h>
#include <UI/ScreenManager.h>



UIScreen::UIScreen(){
    this->selectedElement = nullptr;
    this->selectedIndex = 0;
    this->visible = true;
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
        child->index = this->elements.size() - 1;
        ScreenManager::getInstance()->setDirtyFlag(true);
    }
}

void UIScreen::setVisibility(bool visible){
    this->visible = visible;
    ScreenManager::getInstance()->setDirtyFlag(true);

}

void UIScreen::setSelectedIndex(uint8_t index){
    if(index > this->elements.size()){index = 0;}
    if(index < 0){index = this->elements.size();}
    if(this->selectedElement)this->selectedElement->setState(UIState::BASE);
    
    this->selectedElement = nullptr;
    for (std::pair<std::string, UIElement *> c : this->elements){
        Serial.print("ElIndice: ");
        Serial.print(c.second->index);
        Serial.print("\n");
        if(c.second->index == index){
            this->selectedElement = c.second;
            this->selectedElement->setState(UIState::HOVERED);
            this->selectedIndex = index;
            break;
        }
    }
   
    Serial.print("Indice: ");
    Serial.print(this->selectedIndex);
    Serial.print("\n");

    ScreenManager::getInstance()->setDirtyFlag(true);
}


void UIScreen::onButtonPress(uint8_t button){
    if(!visible){return;}
    if(selectedElement){Serial.println(selectedElement->index);}
    else{Serial.println("semelemento");}
    if(selectedElement){selectedElement->onButtonPress(button); return;}
}

void UIScreen::render(Adafruit_GFX* tft, int stripOffset){
    if(!visible){return;}
    for (std::pair<std::string, UIElement *> c : this->elements){c.second->render(tft, stripOffset);}

}