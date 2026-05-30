#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <future>
#include <vector>

#define CONNECTION_TIMEOUT 10000

enum HTTPMETHOD { 
    GET,
    POST
};
class WifiManager {
private:
    static WifiManager* _instance;
    

    WifiManager();

public:
    static WifiManager* getInstance();

    bool isConnected;

    std::future<String> Fetch(String URI, HTTPMETHOD method, const String& payload);

    std::future<String> Fetch(String URI);

    std::vector<String> GetAvaliableWifis();
    bool Connect(const String SSID, const String& password);
};

#endif