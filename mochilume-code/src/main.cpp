#include <Arduino.h>
#include "HalConfig.h"
#include "DisplayManager.h"
#include "ActivityManager.h"
#include "Test.h"

DisplayManager* display;
ActivityManager* activity;

Test* testActivity = new Test();

void setup() {
    Serial.begin(115200);
    delay(2000); //delay pra dar tempo do platformio abrir o serial e n perder os prints
    display = DisplayManager::getInstance();
    display->begin();
    activity = ActivityManager::getInstance();
    activity->registerActivity(testActivity);
    activity->setActivity("test");
    Serial.println("Mochilume OS: Setup Finalizado.");
}

void loop() {
    activity->loopActivity();
}