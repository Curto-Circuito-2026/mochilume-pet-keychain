#include "UI/UIInput.h"
#include <HalConfig.h>
#include "Assets.h"
UIStyle charStyle = {
    15,15,
    5,
    COLOR_BACKGROUND_BLUE,
    false,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_TEXT_MINT,
    2,
    TextAlign::CENTER  
};
UIStyle charStyleHover = {
    15,15,
    5,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_BACKGROUND_BLUE,
    2,
    TextAlign::CENTER  
};
UIStyle inputStyle = {
    0,0,
    0,
    0,
    true,
    nullptr,
    0,
    0,0,
    0,0,
    0,
    0,
    TextAlign::LEFT
};

UIInput::UIInput(std::string id, int16_t x, int16_t y, int charSize, String initialValue) : UIElement(id, x, y, inputStyle, inputStyle, inputStyle) {
   this->charSize = charSize;
   this->value = initialValue;
   this->value.toLowerCase();
   while (this->value.length() < charSize) {
       this->value += " ";
   }
   for(int i = 0; i < charSize; i++){
        int x = 15*i; 
        UIElement* character = new UIElement(String(i).c_str(), x, 0, charStyle, charStyleHover, charStyle);

        character->setAction(BTN_UP, [this, i](UIElement* element){
            if(element->getText() == " "){element->setText("z"); return;}
            char val = element->getText().charAt(0);
            int ascii = static_cast<int>(val);
            int finalVal = ascii - 1;
            if(finalVal < static_cast<int>('a')){element->setText(" ");return;}
            element->setText(String((char)finalVal));
            this->value[i] = finalVal;
        });
        character->setAction(BTN_DOWN, [this, i](UIElement* element){
            if(element->getText() == " "){element->setText("a"); return;}
            char val = element->getText().charAt(0);
            int ascii = static_cast<int>(val);
            int finalVal = ascii + 1;
            if(finalVal > static_cast<int>('z')){element->setText(" ");return;}
            element->setText(String((char)finalVal));
            this->value[i] = finalVal;
        });

        if(i < value.length()){character->setText(String(value.charAt(i)));}
        else {character->setText(" ");}

        this->addChild(character);
   }
   this->setSelectedIndex(0);
}

String UIInput::getValue(){
    String ret = this->value;
    ret.trim();
    return ret;
}

void UIInput::setValue(String value){
    this->value = value;
    while (this->value.length() < this->charSize) {
       this->value += " ";
    }
    for(int i = 0; i < charSize; i++){
        UIElement* character = this->children[String(i).c_str()];
        if(i < value.length()){Serial.print(i);Serial.println(String(value.charAt(i))); character->setText(String(value.charAt(i)));}
        else {character->setText(" ");}
    }

}

bool UIInput::onButtonPress(uint8_t button){
    if(button == BTN_RIGHT){this->setSelectedIndex(selectedIndex + 1);return true;}
    if(button == BTN_LEFT){this->setSelectedIndex(selectedIndex - 1);return true;}

    bool did = this->selectedChild->onButtonPress(button);
    if(did){return true;}
    else{
        if(this->actions[button]){
            this->actions[button](this);
            return true;
        }
    }

    return false;
    
    
}