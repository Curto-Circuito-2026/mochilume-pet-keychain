#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "HalConfig.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "UISystem.h"

class UISystem;
class UIList;
class ActivityManager;

class Activity {
    protected:
        Adafruit_GC9A01A* _tft;
        DisplayManager* _display;
        UISystem* _uiSystem;
        InputManager* _input;
        ActivityManager* _activityManager;
        void initUISystem();
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