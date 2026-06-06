#ifndef UIKeyboard_H
#define UIKeyboard_H

#include "UI/UIElement.h"
#include "UI/UIMenu.h"
#include <functional>

extern UIStyle keyStyle;
extern UIStyle keyStyleHover;

extern UIStyle capsLockStyle;
extern UIStyle capsLockStyleHover;

extern const char* keyMap[5][13];
extern const char* capsKeyMap[5][13];

class UIKeyboard : public UIElement {
    private:
        bool isOpen;
        bool capsOn;
        UIMenu* keyboard;
        UIElement* keyVisualizer;

        void toggleCaps(bool caps);
    public:

        std::function<void(String)> onCloseCallback = nullptr;
        void onClose(std::function<void(String)> callback) { this->onCloseCallback = callback; }

        UIKeyboard(std::string id, int16_t x, int16_t y);
        bool onButtonPress(uint8_t button) override;
        void toggle(bool open);
        String getText();
};

#endif