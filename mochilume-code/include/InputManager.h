#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "HalConfig.h"

struct MPUData {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
};

class InputManager {
private:
    static InputManager* _instance;
    Adafruit_MPU6050 _mpu;
    MPUData _mpuData;
    bool _btnStates[6]; 

    InputManager();

public:
    static InputManager* getInstance();

    bool begin();
    void update();

    bool isPressed(uint8_t pin);
    MPUData getIMU() { return _mpuData; }
    
    //fazer quando chegar modulo
    void handleLoRa(); 
};

#endif