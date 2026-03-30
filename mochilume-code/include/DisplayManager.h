#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include "HalConfig.h"

class DisplayManager {
private:
    //toma esse singleton pattern #unity #games
    Adafruit_GC9A01A* _tft;
    static DisplayManager* _instance;
    DisplayManager();

public:
    static DisplayManager* getInstance();
    
    bool begin();
    Adafruit_GC9A01A* getTFT();
    
    void clear();
    bool isPointInside(int16_t x, int16_t y);
};

#endif