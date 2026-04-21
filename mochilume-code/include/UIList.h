#ifndef UI_LIST_H
#define UI_LIST_H

#include "UIOption.h"
#include <Adafruit_GFX.h>
#include <vector>

class UIList {
private:
    std::vector<UIOption*> options;
    uint8_t curOptionIndex;
    uint8_t zIndex;
    
public:
    UIList(uint8_t zIndex = 0);
    virtual ~UIList();
    
    void addOption(UIOption* option);
    void addOption(const char* label, UICallback callback = nullptr);
    void removeOption(uint8_t index);
    void clear();
    
    uint8_t getCurOptionIndex() const { return curOptionIndex; }
    void setCurOptionIndex(uint8_t index);
    
    UIOption* getCurOption() const;
    UIOption* getOption(uint8_t index) const;
    uint8_t getOptionCount() const { return options.size(); }
    
    uint8_t getZIndex() const { return zIndex; }
    void setZIndex(uint8_t z) { zIndex = z; }
    
    void nextOption();
    void prevOption();
    void selectCurrent();
    
    virtual void render(Adafruit_GFX* gfx) = 0;
};

#endif
