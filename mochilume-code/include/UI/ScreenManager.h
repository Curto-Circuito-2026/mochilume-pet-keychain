#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "UIScreen.h"

class ScreenManager {
private:
    static ScreenManager* _instance;
    ScreenManager();
    ~ScreenManager();
    
    UIScreen* curScreen;
    uint16_t* _psramBuffer = nullptr;
    GFXcanvas16* canvas;

    bool dirty;
public:
    static ScreenManager* getInstance();
    void changeScreen(UIScreen* screen);

    void setDirtyFlag(bool dirty);

    void onButtonPress(uint8_t button);
    void render();
};

#endif