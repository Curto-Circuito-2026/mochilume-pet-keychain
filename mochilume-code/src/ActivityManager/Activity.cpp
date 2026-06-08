#include "Activity.h"
#include "ActivityManager.h"

Activity::Activity(const char* name, const Sprite* icon) 
    : name(name), icon(icon) {
    this->icon = icon;
    _display = DisplayManager::getInstance();
    _tft = _display->getTFT();
    _input = InputManager::getInstance();
    _activityManager = ActivityManager::getInstance();
}
