#include "SimpleUIList.h"
#include <Adafruit_GC9A01A.h>

SimpleUIList::SimpleUIList(uint8_t zIndex, uint16_t x, uint16_t y,
                           uint16_t itemWidth, uint16_t itemHeight,
                           uint16_t bgColor, uint16_t textColor,
                           uint16_t selectionColor)
    : UIList(zIndex), x(x), y(y), itemWidth(itemWidth), itemHeight(itemHeight),
      bgColor(bgColor), textColor(textColor), selectionColor(selectionColor), 
      activeColor(GC9A01A_YELLOW) {}

void SimpleUIList::setItemColor(uint8_t index, uint16_t color) {
    while (itemColors.size() <= index) {
        itemColors.push_back(selectionColor);
    }
    itemColors[index] = color;
}

uint16_t SimpleUIList::getItemColor(uint8_t index) const {
    if (index < itemColors.size()) {
        return itemColors[index];
    }
    return bgColor;  
}

void SimpleUIList::resetItemColors() {
    itemColors.clear();
}

void SimpleUIList::render(Adafruit_GFX* gfx) {
    if (!gfx || getOptionCount() == 0) {
        return;
    }
    
    for (uint8_t i = 0; i < getOptionCount(); i++) {
        UIOption* opt = getOption(i);
        if (!opt) continue;
        
        uint16_t itemY = y + (i * itemHeight);
        uint16_t displayColor;
        
        if (i == getCurOptionIndex()) {
            displayColor = selectionColor;  
        } else {
            displayColor = bgColor;
        }
        
        gfx->fillRect(x, itemY, itemWidth, itemHeight, displayColor);
        
        gfx->drawRect(x, itemY, itemWidth, itemHeight, textColor);
        
        int16_t textX = x + 5;
        int16_t textY = itemY + itemHeight / 2 - 4;
        gfx->setTextColor(GC9A01A_WHITE);
        gfx->setTextSize(1);
        gfx->setCursor(textX, textY);
        gfx->println(opt->label);
    }
}

