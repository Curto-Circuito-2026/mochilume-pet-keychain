#include "ActivityManager.h"
#include "UISystem.h"

ActivityManager* ActivityManager::_instance = nullptr;

ActivityManager::ActivityManager() {
    this->curActivity = nullptr;
    this->_uiSystem = UISystem::getInstance();
}
ActivityManager::~ActivityManager() {
    this->activityMap.clear();
}

ActivityManager* ActivityManager::getInstance() {
    if (_instance == nullptr) _instance = new ActivityManager();
    return _instance;
}

Activity* ActivityManager::getActivityByName(const char* name){
    Activity* retAct = activityMap[name];
    
    return retAct;
}

void ActivityManager::registerActivity(Activity* activity){
    this->activityMap[activity->name] = activity;
}

void ActivityManager::setActivity(const char* name){
    Activity* nextActivity = getActivityByName(name);
    if(nextActivity && curActivity != nextActivity){
        if(curActivity) curActivity->stop();
        curActivity = nextActivity;
        curActivity->setup();
    }
}

void ActivityManager::loopActivity(){
    if(curActivity){curActivity->loop();}
}

void ActivityManager::renderUI(){
    if(_uiSystem){
        _uiSystem->renderAll(DisplayManager::getInstance()->getTFT());
    }
}
