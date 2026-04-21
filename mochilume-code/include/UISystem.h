#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include "UIList.h"
#include "DisplayManager.h"
#include <vector>

class UISystem {
private:
    static UISystem* _instance;
    std::vector<UIList*> uiLists;
    
    UISystem();
    
public:
    static UISystem* getInstance();
    ~UISystem();
    
    void pushUIList(UIList* list);
    void popUIList();
    UIList* getTopUIList() const;
    
    void renderAll(Adafruit_GFX* gfx);
    
    void handleNextButton();
    void handlePrevButton();
    void handleOKButton();
    
    uint8_t getUIListCount() const { return uiLists.size(); }
    bool hasActiveUI() const { return uiLists.size() > 0; }
    
    void clear();
};

#endif
