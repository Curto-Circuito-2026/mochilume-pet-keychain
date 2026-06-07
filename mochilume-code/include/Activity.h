#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "HalConfig.h"
#include "DisplayManager.h"
#include "UI/ScreenManager.h"
#include "InputManager.h"

class ActivityManager;
class UISystem;
class UIList;

class Activity {
    protected:
        DisplayManager* _display = nullptr;
        ActivityManager* _activityManager = nullptr;
        UISystem* _uiSystem = nullptr;
        Adafruit_GC9A01A* _tft = DisplayManager::getInstance()->getTFT();
        ScreenManager* _screen = ScreenManager::getInstance();
        InputManager* _input = InputManager::getInstance();
        std::map<std::string, UIScreen*> screens;
        
    public: 
        const char* name;
        const uint8_t* icon;

        Activity(const char* name, const uint8_t* icon);
        virtual ~Activity() {};
        
        virtual void setup() = 0;
        virtual void loop() = 0;
        virtual void stop() = 0;
        
        UISystem* getUISystem();
        void pushUIList(UIList* list);
        void popUIList();
};

#endif