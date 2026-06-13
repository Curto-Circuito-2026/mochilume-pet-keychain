#ifndef MENU_H
#define MENU_H
#include "Activity.h"
#include <Adafruit_GC9A01A.h>

class Menu : public Activity {
private:
   


    int lastSteps = -1;
    int lastBattery = -1;
    void loadActivities();
public:
    Menu();

    int curApp;
    
    UIElement* mainApp;
    UIElement* appName;
    UIElement* leftApp;
    UIElement* rightApp;
    UIElement* stepsIco;
    UIElement* batteryIco;
    std::vector<Activity*> activities;

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif