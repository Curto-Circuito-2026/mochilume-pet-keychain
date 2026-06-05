#include "WifiManager.h"
#include <WiFi.h>
#include <HTTPClient.h>

WifiManager* WifiManager::_instance = nullptr;

WifiManager::WifiManager() {
    this->isConnected = false;
}

WifiManager* WifiManager::getInstance() {
    if (_instance == nullptr) _instance = new WifiManager();
    return _instance;
}

void WifiManager::FetchTask(void* pvParameters) {
    FetchPayload* data = (FetchPayload*)pvParameters;
    
    HTTPClient http;
    if (http.begin(data->uri)) {
        http.addHeader("Content-Type", "application/json");
        int httpCode = -1;

        switch (data->method) {
            case GET:  httpCode = http.GET(); break;
            case POST: httpCode = http.POST(data->payload); break;
        }

        if (httpCode > 0) {
            *(data->resultDest) = http.getString();
        }
        http.end();
    }

    *(data->finishedFlag) = true;
    delete data; 
    vTaskDelete(NULL); 
}

bool WifiManager::Fetch(String URI, HTTPMETHOD method, const String& payload, String& outResult, bool& outFinished) {
    if (!isConnected) return false;

    outFinished = false;
    outResult = "";

    FetchPayload* data = new FetchPayload();
    data->uri = URI;
    data->method = method;
    data->payload = payload;
    data->resultDest = &outResult;
    data->finishedFlag = &outFinished;

    BaseType_t res = xTaskCreatePinnedToCore(
        WifiManager::FetchTask, // Função da task
        "HTTP_Fetch_Task",     // Nome interno
        4096,                  // Tamanho da Stack (AQUI TÁ A SOLUÇÃO)
        (void*)data,           // Parâmetros enviados
        1,                     // Prioridade
        NULL,                  // Handler
        1                      // Core 1
    );

    return res == pdPASS;
}

bool WifiManager::Fetch(String URI, String& outResult, bool& outFinished) {
    return Fetch(URI, GET, "", outResult, outFinished);
}


std::vector<String> WifiManager::GetAvaliableWifis() {
    std::vector<String> networks;

    int found = WiFi.scanNetworks();
    for (int i = 0; i < found; i++) {
        networks.push_back(WiFi.SSID(i));
    }
   
    WiFi.scanDelete(); 
    return networks;
}

bool WifiManager::Connect(const String SSID, const String& password) {
    WiFi.begin(SSID.c_str(), password.c_str());

    int elapsed = 0;
    while (WiFi.status() != WL_CONNECTED && elapsed < CONNECTION_TIMEOUT) {
        delay(500);
        elapsed += 500;
    }

    this->isConnected = WiFi.status() == WL_CONNECTED;

    if(!this->isConnected) {
        WiFi.disconnect();
    }
    Serial.println(this->isConnected ? "Connected to WiFi" : "Failed to connect to WiFi");
    return this->isConnected;

}