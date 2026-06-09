#include <Arduino.h>
#include "HalConfig.h"
#include "DisplayManager.h"
#include "ActivityManager.h"
#include "InputManager.h"
#include "Activities/Menu.h"
#include "Activities/Config.h"
#include "Activities/Mochilume.h"
#include <WifiManager.h>
#include <SaveManager.h>
#include <LoraManager.h>
#include <LittleFS.h>

DisplayManager* display;
ScreenManager* screen;
ActivityManager* activity;
SaveManager* save;
InputManager* input;
WifiManager* wifiManager;
LoraManager* loraManager;

Menu* menuActivity = nullptr;
Config* configActivity = nullptr;
Mochilume* mochilumeActivity = nullptr;

void setup() {
    Serial.begin(115200);
    LittleFS.begin(true);
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

    Serial.println("FILES START");
    save = SaveManager::getInstance();
    save->begin();
    Serial.println("FILES OK");

    wifiManager = WifiManager::getInstance();
    Serial.println("WIFI OK");
    
    loraManager = LoraManager::getInstance();
    Serial.println("BEFORE BEGIN");
    loraManager->begin();
    Serial.println("LORA OK");
   

    configActivity = new Config();
    activity->registerActivity(configActivity);

    mochilumeActivity = new Mochilume();
    activity->registerActivity(mochilumeActivity);

    menuActivity = new Menu();
    activity->registerActivity(menuActivity);
    
    activity->setActivity("menu");

    Serial.println("Mochilume OS: Setup Finalizado.");
}

void loop() {
    loraManager->loop();
    input->update();
    activity->loopActivity();
}