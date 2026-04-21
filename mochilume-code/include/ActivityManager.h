#ifndef ACTIVITY_MANAGER_H
#define ACTIVITY_MANAGER_H

#include "HalConfig.h"
#include "Activity.h"
#include <map>
#include <string>

class UISystem;

class ActivityManager {
private:
    static ActivityManager* _instance;
    ActivityManager();
    ~ActivityManager();
    std::map<std::string, Activity*> activityMap;
    Activity* getActivityByName(const char* name);
    UISystem* _uiSystem = nullptr;
public:
    static ActivityManager* getInstance();
    Activity* curActivity;
    void setActivity(const char* name);
    void loopActivity();
    void renderUI();
    void registerActivity(Activity* activity);
};

#endif