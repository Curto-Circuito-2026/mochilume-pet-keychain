#ifndef CONFIG_H
#define CONFIG_H
#include "Activity.h"
#include <Adafruit_GC9A01A.h>

class Config : public Activity {
private:
    UIScreen* home;

    UIScreen* wifiData;
    UIScreen* wifiList;
    UIScreen* dataAccount;
    UIScreen* registerAccount;
    UIScreen* loginAccount;

    void createHomeScreen();
    void createWifiDataScreen();
    void createWifiListScreen();
    void createDataAccountScreen();
    void createRegisterAccountScreen();
    void createLoginAccountScreen();

public:
    Config();
    
    void setup() override;
    void loop() override;
    void stop() override;
};

#endif