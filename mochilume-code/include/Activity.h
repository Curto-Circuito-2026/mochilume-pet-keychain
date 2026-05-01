#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "HalConfig.h"
#include "DisplayManager.h"
#include "UI/ScreenManager.h"

class Activity {
    protected:
        Adafruit_GC9A01A* _tft = DisplayManager::getInstance()->getTFT();
        ScreenManager* _screen = ScreenManager::getInstance();
        std::unordered_map<std::string, UIScreen*> screens;
    public: 
        const char* name;
        const uint8_t* icon;

        Activity(const char* name, const uint8_t* icon) : name(name), icon(icon) {}
        virtual ~Activity() {};
        
        virtual void setup() = 0;
        virtual void loop() = 0;
        virtual void stop() = 0;
};

#endif