#include "DisplayManager.h"

DisplayManager* DisplayManager::_instance = nullptr;

DisplayManager::DisplayManager() {
    _tft = new Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RES);
}

DisplayManager* DisplayManager::getInstance() {
    if (_instance == nullptr) _instance = new DisplayManager();
    return _instance;
}

bool DisplayManager::begin() {
    _tft->begin();
    _tft->setRotation(0);
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    clear();
    return true;
}

Adafruit_GC9A01A* DisplayManager::getTFT() {
    return _tft;
}

void DisplayManager::clear() {
    _tft->fillScreen(GC9A01A_BLACK);
}

bool DisplayManager::isPointInside(int16_t x, int16_t y) {
    int32_t dx = x - SCREEN_CENTER;
    int32_t dy = y - SCREEN_CENTER;
    return (dx*dx + dy*dy) <= ((SCREEN_RADIUS-2) * (SCREEN_RADIUS-2));
}