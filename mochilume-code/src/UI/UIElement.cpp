#include "UI/UIElement.h"
#include <HalConfig.h>
#include <UI/ScreenManager.h>



UIElement::UIElement(std::string id, int16_t x, int16_t y, UIStyle style, UIStyle hoverStyle, UIStyle selectedStyle){
    this->id = id;
    this->x = x;
    this->y = y;
    this->visible = true;
    this->style = style;
    this->text = "";
    this->parent = nullptr;
    this->screen = nullptr;
    this->state = UIState::BASE;
    this->selectedChild = nullptr;
    this->hoverStyle = hoverStyle;
    this->selectedStyle = selectedStyle;
}
UIElement::~UIElement(){
    for (std::pair<std::string, UIElement *> c : this->children){delete c.second;}
    this->children.clear();
    ScreenManager::getInstance()->setDirtyFlag(true);
}

void UIElement::setText(String text){
    this->text = text;
    ScreenManager::getInstance()->setDirtyFlag(true);
}

void UIElement::setState(UIState state) {
    this->state = state;
    ScreenManager::getInstance()->setDirtyFlag(true);
}

UIState UIElement::getState(){return this->state;}

void UIElement::setAction(uint8_t action, std::function<void(UIElement*)> function){
    this->actions[action] = function;
}

UIElement* UIElement::getChild(std::string id){return this->children[id];}

void UIElement::addChild(UIElement* child){
    if(this->children[child->id] == nullptr){
        child->setParent(this);
        this->children[child->id] = child;
        ScreenManager::getInstance()->setDirtyFlag(true);
    }
}

void UIElement::setSelectedChild(UIElement* child){
    this->selectedChild = child;
    ScreenManager::getInstance()->setDirtyFlag(true);
}

UIScreen* UIElement::getScreen(){return this->screen;}

void UIElement::setParent(UIElement* parent){
    this->parent = parent;
    setScreen(parent->getScreen());
}
void UIElement::setScreen(UIScreen* screen){
    this->screen = screen;
}

void UIElement::setVisibility(bool visible){
    this->visible = visible;
    ScreenManager::getInstance()->setDirtyFlag(true);

}


void UIElement::setPosition(int16_t x, int16_t y){
    this->x = x;
    this->y = y;
    ScreenManager::getInstance()->setDirtyFlag(true);
}
int16_t* UIElement::getPosition(){
    static int16_t pos[2] = {x,y};
    return pos;
}


void UIElement::setBaseStyle(UIStyle style){this->style = style;ScreenManager::getInstance()->setDirtyFlag(true);}

void UIElement::setHoverStyle(UIStyle style){this->hoverStyle = style;ScreenManager::getInstance()->setDirtyFlag(true);}

void UIElement::setSelectedStyle(UIStyle style){this->selectedStyle = style;ScreenManager::getInstance()->setDirtyFlag(true);}
UIStyle UIElement::getStyle(){
    if(this->state==UIState::HOVERED) {return this->hoverStyle;}
    if(this->state==UIState::SELECTED) {return this->selectedStyle;}
    return this->style;
}

void UIElement::onButtonPress(uint8_t button){
    if(!visible){return;}
    if(selectedChild){selectedChild->onButtonPress(button); return;}


    if(this->actions[button]){
        this->actions[button](this);
    }
}

void UIElement::render(Adafruit_GFX* tft, int stripOffset){
    if(!visible){return;}
    UIStyle s = this->getStyle();
    int16_t renderX = this->x + s.offsetX;
    int16_t renderY = this->y + s.offsetY - stripOffset;

    if(this->parent != nullptr){
       renderX += this->parent->x + this->parent->getStyle().offsetX + this->parent->getStyle().paddingX;
       renderY += this->parent->y + this->parent->getStyle().offsetY + this->parent->getStyle().paddingY;
    }
    tft->fillRect(renderX, renderY, s.width, s.height, s.color);
    if(s.sprite)tft->drawRGBBitmap(renderX, renderY, s.sprite, s.width, s.height);
    if(this->text.length() > 0){
        tft->setTextSize(s.textSize);
        tft->setTextColor(s.textColor);
        tft->setCursor(renderX + s.paddingX, renderY + s.paddingY);
        tft->print(this->text);
    }

    for (std::pair<std::string, UIElement *> c : this->children){c.second->render(tft, stripOffset);}
}