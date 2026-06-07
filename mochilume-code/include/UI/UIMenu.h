#ifndef UIMenu_H
#define UIMenu_H

#include "UI/UIElement.h"

class UIMenu : public UIElement {
    private:
        int columns;
        int rows;
        //0 = top to botton | 1 = left to right
        int direction;
        bool canExit;
    public:
        UIMenu(std::string id, int16_t x, int16_t y, UIStyle style, UIStyle hoverStyle, UIStyle selectedStyle, int columns,  int rows, int direction = 0, bool canExit = false);

        bool onButtonPress(uint8_t button) override;
        void addChild(UIElement* child) override;

};

#endif