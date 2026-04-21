#ifndef SIMPLE_UI_LIST_H
#define SIMPLE_UI_LIST_H

#include "UIList.h"
#include <Adafruit_GC9A01A.h>

class SimpleUIList : public UIList {
private:
    uint16_t x, y;
    uint16_t itemWidth, itemHeight;
    uint16_t bgColor, textColor, selectionColor, activeColor;
    std::vector<uint16_t> itemColors;
    
public:
    SimpleUIList(uint8_t zIndex = 0, uint16_t x = 0, uint16_t y = 0, 
                 uint16_t itemWidth = 120, uint16_t itemHeight = 40,
                 uint16_t bgColor = GC9A01A_BLACK, 
                 uint16_t textColor = GC9A01A_WHITE,
                 uint16_t selectionColor = GC9A01A_GREEN);
    
    virtual ~SimpleUIList() {}
    
    void setPosition(uint16_t x, uint16_t y) { this->x = x; this->y = y; }
    void setItemSize(uint16_t width, uint16_t height) { this->itemWidth = width; this->itemHeight = height; }
    void setColors(uint16_t bg, uint16_t text, uint16_t selection, uint16_t active = GC9A01A_YELLOW) {
        bgColor = bg;
        textColor = text;
        selectionColor = selection;
        activeColor = active;
    }
    
    void setItemColor(uint8_t index, uint16_t color);
    uint16_t getItemColor(uint8_t index) const;
    void resetItemColors();
    
    void render(Adafruit_GFX* gfx) override;
};

#endif
