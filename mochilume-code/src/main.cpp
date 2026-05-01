#include <Arduino.h>
#include "HalConfig.h"
#include "DisplayManager.h"
#include "ActivityManager.h"
#include "InputManager.h"
#include "Test.h"

DisplayManager* display;
ScreenManager* screen;
ActivityManager* activity;
InputManager* input;

Test* testActivity = nullptr;

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("DISPLAY START");
    display = DisplayManager::getInstance();
    display->begin();
    Serial.println("DISPLAY OK");
    
    Serial.println("INPUT START");
    input = InputManager::getInstance();
    input->begin();
    Serial.println("INPUT OK");
    
    Serial.println("ACTIVITY START");
    activity = ActivityManager::getInstance();
    Serial.println("ACTIVITY OK");

    Serial.println("SCREEN START");
    screen = ScreenManager::getInstance();
    Serial.println("SCREEN OK");

    testActivity = new Test();
    activity->registerActivity(testActivity);
    activity->setActivity("test");
    Serial.println("Mochilume OS: Setup Finalizado.");
}

void loop() {
    activity->loopActivity();
}