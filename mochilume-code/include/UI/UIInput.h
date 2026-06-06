#ifndef UIInput_H
#define UIInput_H

#include "UI/UIElement.h"

extern UIStyle charStyle;
extern UIStyle charStyleHover;
extern UIStyle keyboardInputStyle;
class UIInput : public UIElement {
    private:
        int charSize;
        String value;
    public:
        UIInput(std::string id, int16_t x, int16_t y, int charSize, String initialValue);
        bool onButtonPress(uint8_t button) override;
        String getValue();
        void setValue(String value);
};

#endif