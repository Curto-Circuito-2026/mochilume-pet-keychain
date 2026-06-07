#include "Activity.h"
#include "ActivityManager.h"

Activity::Activity(const char* name, const uint8_t* icon) 
    : name(name), icon(icon) {
    _display = DisplayManager::getInstance();
    _tft = _display->getTFT();
    _input = InputManager::getInstance();
    _activityManager = ActivityManager::getInstance();
}
