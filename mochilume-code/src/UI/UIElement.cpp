#include "UI/UIElement.h"
#include <HalConfig.h>
#include <UI/ScreenManager.h>
#include <vector>



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
    this->selectedIndex = 0;
    this->selectedChild = nullptr;
    this->hoverStyle = hoverStyle;
    this->selectedStyle = selectedStyle;
    this->disabled = false;
    this->wrapIndex = true;

    this->onHover = nullptr;
    this->offHover = nullptr;
}
UIElement::~UIElement(){
    for (std::pair<std::string, UIElement *> c : this->children){delete c.second;}
    this->children.clear();
    ScreenManager::getInstance()->setDirtyFlag(true);
}

void UIElement::setDisabled(bool disabled){
    this->disabled = disabled;
    ScreenManager::getInstance()->setDirtyFlag(true);
}
bool UIElement::getDisabled(){
    return this->disabled;
}

int UIElement::getChildAmount(){
    return this->children.size();
}



void UIElement::setText(String text){
    this->text = text;
    ScreenManager::getInstance()->setDirtyFlag(true);
}

void UIElement::setState(UIState state) {
    if(state == UIState::HOVERED){
        if(this->onHover){  
            this->onHover(this);
        }
    }
    if(state == UIState::BASE && this->state == UIState::HOVERED){
        if(this->offHover){this->offHover(this);}
    }
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
        child->index = this->children.size() - 1;
        ScreenManager::getInstance()->setDirtyFlag(true);
    }
}

bool UIElement::getVisibility(){
    return this->visible;
}

String UIElement::getText(){
    return this->text;
}

int UIElement::getIndex(){
    return this->index;
}

void UIElement::setSelectedIndexByName(std::string name) {
    setSelectedIndex(children[name]->getIndex());
}
void UIElement::setSelectedIndex(uint8_t index){
    if(this->children.size() == 0){return;}
    Serial.print("INDICE ESCOLHIDO: ");
    Serial.print(index);
    Serial.print(" | CHILDRENSIZE: ");
    Serial.print(this->children.size());
    Serial.print(" | INDICE ATUAL: ");
    Serial.print(this->selectedIndex);
    Serial.print("\n");

    if(index == -1){
        if(this->selectedChild){
            this->selectedChild->setState(UIState::BASE);
            this->selectedChild->setSelectedIndex(-1);
        }
        this->selectedChild = nullptr;
        this->selectedIndex = index;
        ScreenManager::getInstance()->setDirtyFlag(true);
        return;
    }


    if(index >= this->children.size()){if(wrapIndex) index = 0; else index = this->children.size();}
    if(index < 0){if(wrapIndex) index = index = this->children.size(); else index = 0;}
    if(this->selectedChild){this->selectedChild->setState(UIState::BASE);this->selectedChild->setSelectedIndex(-1);}

    
    Serial.print("INDICE TRATADO: ");
    Serial.print(index);
    Serial.print("\n");

    this->selectedChild = nullptr;
    for (std::pair<std::string, UIElement *> c : this->children){
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
            this->selectedChild = c.second;
            this->selectedChild->setState(UIState::HOVERED);
            this->selectedChild->setSelectedIndex(0);
            this->selectedIndex = index;
            break;
        }
    }

    if(this->selectedChild != nullptr){
        Serial.print("FILHO ATUAL: ");
        Serial.println(this->selectedChild->id.c_str());
    }

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

void UIElement::clearChildren(){
    this->children.clear();
};

bool UIElement::onButtonPress(uint8_t button){
    Serial.print("BOTÃO: ");
    Serial.print(button);
    Serial.print(" EM ELEMENTO: ");
    Serial.print(this->id.c_str());
    Serial.print("\n");

    if(!visible){return false;}
    if(selectedChild){
        bool did = selectedChild->onButtonPress(button); 
        if(did) return true;
    }

    if(this->actions[button]){
        this->actions[button](this);
        return true;
    }
    return false;
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
    if(!s.noFill){tft->fillRoundRect(renderX, renderY, s.width, s.height, s.borderRadius, s.color);}
    if(s.sprite){
        if(s.sprite->mask != nullptr){tft->drawRGBBitmap(renderX, renderY, s.sprite->sprite, s.sprite->mask, s.width, s.height);}
        else{tft->drawRGBBitmap(renderX, renderY, s.sprite->sprite, s.width, s.height);}
    }
    if(this->text.length() > 0){
        tft->setTextSize(s.textSize);
        tft->setTextColor(s.textColor);
        tft->setCursor(renderX + s.paddingX, renderY + s.paddingY);

        if(s.textAlign == TextAlign::CENTER){
            int16_t x1, y1;
            uint16_t w, h;
            tft->getTextBounds(this->text, 0, 0, &x1, &y1, &w, &h);
            int16_t tx = renderX + (s.width  - w) / 2 - x1;
            tft->setCursor(tx, renderY + s.paddingY);
        }
        if(s.textAlign == TextAlign::RIGHT){
            tft->setCursor(s.width - s.paddingX, renderY + s.paddingY);
        }

        tft->print(this->text);
    }


    std::vector<std::pair<std::string, UIElement *>> pairs(this->children.begin(), this->children.end());

    std::sort(pairs.begin(), pairs.end(), [](const std::pair<std::string, UIElement *>& a, const std::pair<std::string, UIElement *>& b) {
        return a.second->getStyle().z < b.second->getStyle().z; 
    });

    for (const std::pair<std::string, UIElement *>& p : pairs) {
        p.second->render(tft, stripOffset);
    }

    // for (std::pair<std::string, UIElement *> c : this->children){c.second->render(tft, stripOffset);}
}