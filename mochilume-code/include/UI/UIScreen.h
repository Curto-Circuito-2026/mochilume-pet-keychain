#ifndef UIScreen_H
#define UIScreen_H

#include <map>
#include <string>
#include <Adafruit_GC9A01A.h>
#include "UIElement.h"

class UIScreen {
private:
    std::map<std::string, UIElement*> elements;
    bool visible;
    UIElement* selectedElement;
    uint8_t selectedIndex;

public:
    uint16_t backgroundColor;
    const uint16_t* backgroundImage;

    UIScreen();
    ~UIScreen();

    void onButtonPress(uint8_t button);

    UIElement* getChild(std::string id);
    void addChild(UIElement* child);

    void setSelectedIndexByName(std::string name);
    void setSelectedIndex(uint8_t index);

    void setVisibility(bool visible);

    void render(Adafruit_GFX* tft, int stripOffset = 0);
};



#endif