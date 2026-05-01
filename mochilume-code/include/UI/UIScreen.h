#ifndef UIScreen_H
#define UIScreen_H

#include <unordered_map>
#include <string>
#include <Adafruit_GC9A01A.h>
#include "UIElement.h"

class UIScreen {
private:
    std::unordered_map<std::string, UIElement*> elements;
    bool visible;
    UIElement* selectedElement;
public:
    UIScreen();
    ~UIScreen();

    void onButtonPress(uint8_t button);

    UIElement* getChild(std::string id);
    void addChild(UIElement* child);

    void setVisibility(bool visible);

    void setSelectedElement(UIElement* element);

    void render(Adafruit_GFX* tft, int stripOffset = 0);
};



#endif