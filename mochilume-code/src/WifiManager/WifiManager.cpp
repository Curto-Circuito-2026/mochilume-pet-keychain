#include "WifiManager.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

WifiManager* WifiManager::_instance = nullptr;

WifiManager::WifiManager() {

    this->isConnected = false;
    this->currentNetwork = "";
    this->savedPassword = "";

    if (LittleFS.exists("/user.json")) {
        File file = LittleFS.open("/user.json", "r");
        
        if (file) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, file);
            
            if (!error) {
                const char* savedSSID = doc["defaultWIFIID"];
                const char* password = doc["defaultWIFIPassword"];
                
                if (savedSSID != nullptr && strlen(savedSSID) > 0) {this->currentNetwork = String(savedSSID);}
                if (password != nullptr && strlen(password) > 0) {this->savedPassword = String(password);}
            } else {
                Serial.print(F("Failed to parse user.json: "));
                Serial.println(error.f_str());
            }
            file.close();
        }
    }
}

WifiManager* WifiManager::getInstance() {
    if (_instance == nullptr) _instance = new WifiManager();
    return _instance;
}

String WifiManager::GetDeviceID() {
    return WiFi.macAddress();
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

void WifiManager::Disconnect(){
    WiFi.disconnect();
}

bool WifiManager::ReConnect(){
    return this->Connect(this->currentNetwork, this->savedPassword);
}
bool WifiManager::Connect(const String SSID, const String& password) {
    WiFi.begin(SSID.c_str(), password.c_str());

    int elapsed = 0;
    while (WiFi.status() != WL_CONNECTED && elapsed < WIFI_CONNECTION_TIMEOUT) {
        delay(500);
        elapsed += 500;
    }

    this->isConnected = WiFi.status() == WL_CONNECTED;
    if(this->isConnected){
        this->currentNetwork = SSID;
        this->savedPassword = password;
        String filePath = "/user.json";
        JsonDocument doc;
        if (LittleFS.exists(filePath)) {
            File file = LittleFS.open(filePath, "r");
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                file.close();
                if (error) {
                    Serial.println(F("erro achar."));
                }
            }
        
        }

        doc["defaultWIFIID"] = SSID;
        doc["defaultWIFIPassword"] = password;
        
        if (!doc.containsKey("steps")) doc["steps"] = 0;
        if (!doc.containsKey("userID")) doc["userID"] = "";

        File file = LittleFS.open(filePath, "w");
        if (!file) {
            Serial.println(F("erro ler"));
            return false;
        }

        if (serializeJson(doc, file) == 0) {
            Serial.println(F("erro escrever"));
            file.close();
            return false;
        }

        file.close();
        Serial.println(F("wifi salvo /user.json"));
    }

    if(!this->isConnected) {
        WiFi.disconnect();
    }
    Serial.println(this->isConnected ? "Connected to WiFi" : "Failed to connect to WiFi");
    return this->isConnected;

}