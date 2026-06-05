#include "UI/UIScreen.h"
#include <HalConfig.h>
#include <UI/ScreenManager.h>
#include <vector>



UIScreen::UIScreen(){
    this->backgroundColor = GC9A01A_BLACK;
    this->backgroundImage = nullptr;
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
     if(index == -1){
        if(this->selectedElement){
            this->selectedElement->setState(UIState::BASE);
            this->selectedElement->setSelectedIndex(-1);
        }
        this->selectedElement = nullptr;
        this->selectedIndex = index;
        ScreenManager::getInstance()->setDirtyFlag(true);
        return;
    }

    if(index >= this->elements.size()){index = 0;}
    if(index < 0){index = this->elements.size();}
    if(this->selectedElement){this->selectedElement->setState(UIState::BASE);this->selectedElement->setSelectedIndex(-1);}
    
    this->selectedElement = nullptr;
    for (std::pair<std::string, UIElement *> c : this->elements){
        if(c.second->index == index){
           if(c.second->getDisabled() || !c.second->getVisibility()){
                if(index > this->selectedIndex){
                    setSelectedIndex(index + 1);
                    return;
                }else{
                    setSelectedIndex(index - 1);
                    return;
                }
            }
            this->selectedElement = c.second;
            this->selectedElement->setState(UIState::HOVERED);
            this->selectedElement->setSelectedIndex(0);
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
    Serial.print("BOTÃO: ");
    Serial.print(button);
    Serial.print(" EM SCREEN\n");

    if(!visible){return;}

    if(selectedElement){
        Serial.print("ELEMENTO SELECIONADO: ");
        Serial.print(selectedElement->index);
        Serial.print(" | ");
        Serial.print(selectedElement->id.c_str());
        Serial.print("\n");

        bool did = selectedElement->onButtonPress(button); 
        if(did) return;
    }
    else{
        Serial.println("SEM ELEMENTO SELECIONADO");
    }

    if(button == BTN_RIGHT){setSelectedIndex(this->selectedIndex + 1);}
    if(button == BTN_LEFT){setSelectedIndex(this->selectedIndex - 1);}
}

void UIScreen::render(Adafruit_GFX* tft, int stripOffset){
    if(!visible){return;}

    std::vector<std::pair<std::string, UIElement *>> pairs(this->elements.begin(), this->elements.end());

    std::sort(pairs.begin(), pairs.end(), [](const std::pair<std::string, UIElement *>& a, const std::pair<std::string, UIElement *>& b) {
        return a.second->getStyle().z < b.second->getStyle().z; 
    });

    for (const std::pair<std::string, UIElement *>& p : pairs) {
        p.second->render(tft, stripOffset);
    }

    // for (std::pair<std::string, UIElement *> c : this->elements){
    //     c.second->render(tft, stripOffset);
    // }

}