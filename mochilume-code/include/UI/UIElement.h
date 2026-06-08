#ifndef UIElement_H
#define UIElement_H

#include <map>
#include <string>
#include <Adafruit_GC9A01A.h>
#include "Assets.h"
#include "UIStyle.h"


enum class UIState {
    BASE, HOVERED, SELECTED
};

class UIScreen;

class UIElement {
protected:
    String text;
    UIElement* parent;
    UIScreen* screen;
    std::map<std::string, UIElement*> children;

    bool wrapIndex;

    int16_t x,y;
    bool visible;
    UIStyle style;
    UIStyle hoverStyle;
    UIStyle selectedStyle;

    uint8_t selectedIndex;

    UIState state;
    UIElement* selectedChild;
    
    bool disabled;



    std::map<uint8_t, std::function<void(UIElement*)>> actions;
public:
    std::string id;
    uint8_t index;
    UIElement(std::string id, int16_t x, int16_t y, UIStyle style, UIStyle hoverStyle, UIStyle selectedStyle);
    ~UIElement();

    std::function<void(UIElement*)> onHover;
    std::function<void(UIElement*)> offHover;

    void setState(UIState state);
    UIState getState();

    void setDisabled(bool disabled);
    bool getDisabled();

    UIScreen* getScreen();

    void setSelectedIndex(uint8_t index);
    int getIndex();
    void setSelectedIndexByName(std::string name);
    
    void setParent(UIElement* parent);
    void setScreen(UIScreen* screen);

    virtual bool onButtonPress(uint8_t button);

    UIElement* getChild(std::string id);
    virtual void addChild(UIElement* child);
    void clearChildren();
    int getChildAmount();

    void setPosition(int16_t x, int16_t y);
    int16_t* getPosition();

    void setVisibility(bool visible);
    bool getVisibility();

    void setBaseStyle(UIStyle style);
    void setHoverStyle(UIStyle style);
    void setSelectedStyle(UIStyle style);

    UIStyle getStyle();

    void setText(String text);
    String getText();

    void setAction(uint8_t action, std::function<void(UIElement*)> function);

    void render(Adafruit_GFX* tft, int stripOffset = 0);
};



#endif