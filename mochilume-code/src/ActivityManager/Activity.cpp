#include "Activity.h"
#include "UISystem.h"
#include "ActivityManager.h"

Activity::Activity(const char* name, const uint8_t* icon) 
    : name(name), icon(icon) {
    _display = DisplayManager::getInstance();
    _tft = _display->getTFT();
    _input = InputManager::getInstance();
    _activityManager = ActivityManager::getInstance();
    _uiSystem = nullptr;
}

void Activity::initUISystem() {
    if (_uiSystem == nullptr) {
        _uiSystem = UISystem::getInstance();
    }
    if (_activityManager == nullptr) {
        _activityManager = ActivityManager::getInstance();
    }
}

UISystem* Activity::getUISystem() {
    initUISystem();
    return _uiSystem;
}

void Activity::pushUIList(UIList* list) {
    UISystem* ui = getUISystem();
    if (ui) {
        ui->pushUIList(list);
    }
}

void Activity::popUIList() {
    UISystem* ui = getUISystem();
    if (ui) {
        ui->popUIList();
    }
}

