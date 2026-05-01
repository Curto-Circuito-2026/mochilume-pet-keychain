#ifndef UIElement_H
#define UIElement_H

#include <unordered_map>
#include <string>
#include <Adafruit_GC9A01A.h>

struct UIStyle {
    int16_t width, height;
    uint16_t color;
    uint16_t* sprite;
    
    int16_t z;

    int16_t offsetX, offsetY;
    int8_t paddingX, paddingY;

    uint16_t textColor;
    uint8_t textSize;
};

enum class UIState {
    BASE, HOVERED, SELECTED
};

class UIScreen;

class UIElement {
private:
    String text;
    UIElement* parent;
    UIScreen* screen;
    std::unordered_map<std::string, UIElement*> children;

    int16_t x,y;
    bool visible;
    UIStyle style;
    UIStyle hoverStyle;
    UIStyle selectedStyle;

    UIState state;
    UIElement* selectedChild;

    std::unordered_map<uint8_t, std::function<void(UIElement*)>> actions;
public:
    std::string id;
    UIElement(std::string id, int16_t x, int16_t y, UIStyle style, UIStyle hoverStyle, UIStyle selectedStyle);
    ~UIElement();

    void setState(UIState state);
    UIState getState();

    UIScreen* getScreen();

    void setSelectedChild(UIElement* child);

    void setParent(UIElement* parent);
    void setScreen(UIScreen* screen);

    void onButtonPress(uint8_t button);

    UIElement* getChild(std::string id);
    void addChild(UIElement* child);

    void setPosition(int16_t x, int16_t y);
    int16_t* getPosition();

    void setVisibility(bool visible);

    void setBaseStyle(UIStyle style);
    void setHoverStyle(UIStyle style);
    void setSelectedStyle(UIStyle style);

    UIStyle getStyle();

    void setText(String text);

    void setAction(uint8_t action, std::function<void(UIElement*)> function);

    void render(Adafruit_GFX* tft, int stripOffset = 0);
};



#endif