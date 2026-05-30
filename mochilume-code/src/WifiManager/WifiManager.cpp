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


std::future<String> WifiManager::Fetch(String URI, HTTPMETHOD method, const String& payload){
    return std::async(std::launch::async, [=]() {
        String result;
        if(isConnected){
            HTTPClient http;
            http.begin(URI);
            http.addHeader("Content-Type", "application/json");
            int httpCode = -1;

            switch (method) {
                case GET:    httpCode = http.GET();        break;
                case POST:   httpCode = http.POST(payload);   break;
            }

            if (httpCode > 0) {
                result = http.getString();
            }


            http.end();
        }
        return result;
    });
}
std::future<String> WifiManager::Fetch(String URI){
        return std::async(std::launch::async, [=]() {
        String result;
        if(isConnected){
            HTTPClient http;
            http.begin(URI);
            http.addHeader("Content-Type", "application/json");

            int httpCode = http.GET();
           
            if (httpCode > 0) {
                result = http.getString();
            }


            http.end();
        }
        return result;
    });
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

    return this->isConnected;

}