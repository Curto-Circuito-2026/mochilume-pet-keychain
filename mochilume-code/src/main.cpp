#include <Arduino.h>
#include "HalConfig.h"
#include "DisplayManager.h"
#include "ActivityManager.h"
#include "InputManager.h"
#include "Test.h"
#include "Activities/Menu.h"
#include "Activities/Config.h"
#include "Activities/Mochilume.h"
#include <WifiManager.h>


DisplayManager* display;
ScreenManager* screen;
ActivityManager* activity;
InputManager* input;
WifiManager* wifiManager;

Test* testActivity = nullptr;
Menu* menuActivity = nullptr;
// Config* configActivity = nullptr;
Mochilume* mochilumeActivity = nullptr;

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


    wifiManager = WifiManager::getInstance();
    Serial.println("WIFI OK");

    std::vector<String> availableWifis = wifiManager->GetAvaliableWifis();
    wifiManager->Connect("APT12", "Bunny1504");


    const char* novoUsuario = "ArthurPalladino";
    const char* novaSenha   = "SenhaSegura123";

    char jsonPayload[128];
    snprintf(jsonPayload, sizeof(jsonPayload), "{\"userName\":\"%s\",\"password\":\"%s\"}", novoUsuario, novaSenha);

    Serial.println("Tentando registrar jogador...");
    Serial.print("Payload de envio: ");
    Serial.println(jsonPayload);

    String respostaRegistro = "";
    bool registroTerminou = false;

    wifiManager->Fetch(AUTH_REGISTER_ENDPOINT, POST, jsonPayload, respostaRegistro, registroTerminou);

    while (!registroTerminou) {
        delay(10);
    }

    Serial.println("--- RESULTADO DO REGISTRO ---");
    if (respostaRegistro.length() > 0) {
        Serial.print("Resposta do Servidor: ");
        Serial.println(respostaRegistro.c_str());
    } else {
        Serial.println("Registro enviado! Verifique se o status do HTTP no servidor retornou 200 OK.");
    }




    // configActivity = new Config();
    // activity->registerActivity(configActivity);

    mochilumeActivity = new Mochilume();
    activity->registerActivity(mochilumeActivity);

    menuActivity = new Menu();
    activity->registerActivity(menuActivity);
    
    activity->setActivity("test");

    Serial.println("Mochilume OS: Setup Finalizado.");
}

void loop() {
    input->update();
    activity->loopActivity();
}