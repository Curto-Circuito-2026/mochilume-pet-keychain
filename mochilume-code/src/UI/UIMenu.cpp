#include "UI/UIMenu.h"
#include <HalConfig.h>

UIMenu::UIMenu(std::string id, int16_t x, int16_t y, UIStyle style, UIStyle hoverStyle, UIStyle selectedStyle, int columns, int rows, int direction, bool canExit) : UIElement(id, x, y, style, hoverStyle, selectedStyle) {
    this->columns = columns;
    this->rows = rows;
    this->direction = direction;
    this->canExit = canExit;
}

void UIMenu::addChild(UIElement* child){
    UIElement::addChild(child); 
    
    if(this->direction == 1){
        this->columns +=1;
    }
}

bool UIMenu::onButtonPress(uint8_t button){
    Serial.print("BOTÃO: ");
    Serial.print(button);
    Serial.print(" EM ELEMENTO: ");
    Serial.print(this->id.c_str());
    Serial.print("\n");

    if(this->actions[button]){
        this->actions[button](this);
        return true;
    }

    if(this->state == UIState::SELECTED){
        Serial.println("SELECTED");
        if(button == BTN_B && canExit) {this->setState(UIState::HOVERED); return true;}
        if(button == BTN_A){this->selectedChild->onButtonPress(button);return true;}
        

        if(button == BTN_RIGHT){this->setSelectedIndex(selectedIndex + 1);return true;}
        if(button == BTN_LEFT){this->setSelectedIndex(selectedIndex - 1);return true;}
        if(button == BTN_UP){this->setSelectedIndex(selectedIndex - columns);return true;}
        if(button == BTN_DOWN){this->setSelectedIndex(selectedIndex + columns);return true;}

    }else{
        Serial.println("NÃO SELECTED");
        if(button == BTN_A){this->setState(UIState::SELECTED); return true;}
    }
    return false;
    
    
}