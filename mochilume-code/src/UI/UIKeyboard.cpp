#include "UI/UIKeyboard.h"
#include "UI/UIMenu.h"
#include <HalConfig.h>
#include "Assets.h"

UIStyle keyStyle = {
    10,10,
    2,
    COLOR_BACKGROUND_BLUE,
    false,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER  
};
UIStyle keyStyleHover = {
    10,10,
    2,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER  
};

UIStyle capsLockStyle = {
    30,10,
    2,
    COLOR_BACKGROUND_BLUE,
    false,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER  
};

UIStyle capsLockStyleHover = {
    30,10,
    2,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER 
};

UIStyle centerText = {
    240,0,
    0,
    0,
    true,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER,
};

const char* keyMap[5][13] = {
    { "'",  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=" },
    { "q",  "w", "e", "r", "t", "y", "u", "i", "o", "p", "´", "[", ""  },
    { "a",  "s", "d", "f", "g", "h", "j", "k", "l", "ç", "~", "]", ""  },
    { "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", ";", "/", ""  },
    { ""  ,  "",  "",  "",  "CAPS", " ", "BACKSPACE", "ENTER", "", "", "", "", ""  }
};

const char* capsKeyMap[5][13] = {
    { "\"", "!", "@", "#", "$", "%", "¬", "&", "*", "(", ")", "_", "+" },
    { "Q",  "W", "E", "R", "T", "Y", "U", "I", "O", "P", "`", "{", ""  },
    { "A",  "S", "D", "F", "G", "H", "J", "K", "L", "Ç", "^", "}", ""  },
    { "|",  "Z", "X", "C", "V", "B", "N", "M", "<", ">", ":", "?", ""  },
    { ""  ,  "",  "",  "",  "CAPS", " ", "BACKSPACE", "ENTER", "", "", "", "", ""  }

};

UIKeyboard::UIKeyboard(std::string id, int16_t x, int16_t y) : UIElement(id, x, y, backdrop, backdrop, backdrop){
    keyVisualizer = new UIElement("visualizer", 0,80, centerText, centerText, centerText);
    keyboard = new UIMenu("keyboard", 60, 120, backdrop, backdrop, backdrop, 13, 5);
    this->capsOn = false;
    for(int r = 0; r < 5; r++){
        for(int c = 0; c < 13; c++){
            UIElement* key = new UIElement((String(r) + "|" + String(c)).c_str(), (c*10), (r*10), keyMap[r][c][0] != '\0' ? keyStyle : emptyStyle, keyStyleHover, keyStyle);
            if(keyMap[r][c][0] == '\0'){key->setDisabled(true); key->setVisibility(false);}

            if(keyMap[r][c] == "CAPS"){
                key->setBaseStyle(capsLockStyle);
                key->setHoverStyle(capsLockStyleHover);
                key->setPosition(c*10 - 20, r*10);
                key->setAction(BTN_A,[this, r, c](UIElement* element){
                    this->toggleCaps(!this->capsOn);
                });
                key->setText(keyMap[r][c]);
            }
            else if(keyMap[r][c] == "BACKSPACE"){
                key->setBaseStyle(capsLockStyle);
                key->setHoverStyle(capsLockStyleHover);
                key->setAction(BTN_A,[this, r, c](UIElement* element){
                    String text = this->keyVisualizer->getText().substring(0, this->keyVisualizer->getText().length() - 1);
                    this->keyVisualizer->setText(text);
                });
                key->setText(keyMap[r][c]);

            }else if(keyMap[r][c] == "ENTER"){
                key->setBaseStyle(capsLockStyle);
                key->setHoverStyle(capsLockStyleHover);
                key->setPosition(c*10 + 20, r*10);
                key->setAction(BTN_A,[this](UIElement* element){
                    this->toggle(false);
                    if(this->onCloseCallback) {
                        this->onCloseCallback(this->getText());
                    }
                });
                key->setText(keyMap[r][c]);
            }
            
            else if(keyMap[r][c][0] != '\0'){
                key->setAction(BTN_A,[this, r, c](UIElement* element){
                    const char* letter = this->capsOn ? capsKeyMap[r][c] : keyMap[r][c];
                    String text = this->keyVisualizer->getText() + letter;
                    this->keyVisualizer->setText(text);
                });
                key->setText(keyMap[r][c]);
            }
            keyboard->addChild(key);
        }
    }
    this->addChild(keyboard);
    this->addChild(keyVisualizer);
    this->setSelectedIndex(0);
    keyboard->setState(UIState::SELECTED);
    keyboard->setSelectedIndex(0);

    this->toggle(false);
}

void UIKeyboard::toggleCaps(bool caps){
    this->capsOn = caps;
    
    for(int r = 0; r < 5; r++){
        for(int c = 0; c < 13; c++){
            UIElement* key = this->keyboard->getChild((String(r) + "|" + String(c)).c_str());
            if (key) {
                key->setText(capsOn ? capsKeyMap[r][c] : keyMap[r][c]);
            }
        }
    }
}

String UIKeyboard::getText(){
    return this->keyVisualizer->getText();
}
void UIKeyboard::toggle(bool open){
    if(open) {
        this->keyVisualizer->setText(""); 
        this->setSelectedIndex(0);
        keyboard->setState(UIState::SELECTED);
    }
    this->setVisibility(open);
    this->isOpen = open;
}

bool UIKeyboard::onButtonPress(uint8_t button){
    if(button == BTN_B && isOpen){
        toggle(false); 
        if(this->onCloseCallback) {
            this->onCloseCallback(this->getText());
        }
        return true;
    }
    else if(button == BTN_A && !isOpen){toggle(true); return true;}
    else{
        bool did = this->selectedChild->onButtonPress(button);
        if(did){return true;}
        if(this->actions[button]){this->actions[button](this);return true;}
    }
    return false;
    
}