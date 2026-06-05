#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <vector>

#define CONNECTION_TIMEOUT 10000

enum HTTPMETHOD { GET, POST };

// Estrutura para passar dados para a Task do FreeRTOS
struct FetchPayload {
    String uri;
    HTTPMETHOD method;
    String payload;
    String* resultDest;
    bool* finishedFlag;
};

class WifiManager {
private:
    static WifiManager* _instance;
    WifiManager();
    
    static void FetchTask(void* pvParameters);

public:
    static WifiManager* getInstance();
    bool isConnected;

    bool Fetch(String URI, HTTPMETHOD method, const String& payload, String& outResult, bool& outFinished);
    bool Fetch(String URI, String& outResult, bool& outFinished);

    std::vector<String> GetAvaliableWifis();
    bool Connect(const String SSID, const String& password);
};

#endif