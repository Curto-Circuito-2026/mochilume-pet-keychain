#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "HalConfig.h"
#include <queue>

struct MPUData {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
};

extern uint8_t buttons[];

class InputManager {
private:
    static InputManager* _instance;
    Adafruit_MPU6050 _mpu;
    MPUData _mpuData;
    bool _btnStates[6]; 

    volatile unsigned long passosValidados;
    int passosCandidatos;
    unsigned long tempoUltimoPasso;
    int estadoAtual;
    bool mpuInitialized;

    int passosLocaisBatch;
    static const int AMOSTRAS_FILTRO = 10;
    float historicoMagnitude[AMOSTRAS_FILTRO];
    int indiceFiltro;
    int bateryLevel;

    void readIMU();           
    void processPedometer();

public:
    static InputManager* getInstance();
    unsigned long getSteps();
    bool begin();
    void update();

    std::deque<int> inputQueue;

    bool isPressed(uint8_t pin);
    MPUData getIMU() { return _mpuData; }
    void updateBatteryLevel();
    int getBatteryLevel() { return bateryLevel; };
    void setSteps(int quantidadeSalvar);
    void saveSteps(int quantidadeSalvar);
    void saveStepsInFile();
    
};

#endif