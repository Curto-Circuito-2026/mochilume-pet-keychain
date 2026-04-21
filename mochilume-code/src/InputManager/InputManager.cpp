#include "InputManager.h"
#include "HalConfig.h"

InputManager* InputManager::_instance = nullptr;

InputManager::InputManager() {}

InputManager* InputManager::getInstance() {
    if (_instance == nullptr) _instance = new InputManager();
    return _instance;
}

bool InputManager::begin() {
    uint8_t buttons[] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_A, BTN_B};
    for(uint8_t btn : buttons) {
        // oq tava escrito aqui era mentira!!!!!
        // coloque resistor de 10k nos seus botoes
        pinMode(btn, INPUT_PULLDOWN);
    }

    if (!_mpu.begin()) {
        Serial.println("Falha ao encontrar MPU6050");
        return false;
    }
    
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    return true;
}

void InputManager::update() {
    _btnStates[0] = !digitalRead(BTN_UP);
    _btnStates[1] = !digitalRead(BTN_DOWN);
    _btnStates[2] = !digitalRead(BTN_LEFT);
    _btnStates[3] = !digitalRead(BTN_RIGHT);
    _btnStates[4] = !digitalRead(BTN_A);
    _btnStates[5] = !digitalRead(BTN_B);

    // 2. Leitura da IMU
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    _mpuData.accX = a.acceleration.x;
    _mpuData.accY = a.acceleration.y;
    _mpuData.accZ = a.acceleration.z;
    
    //Esboço de lógica para contar passos
    // float totalAcc = sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2));
    // if (totalAcc > 12.0f) 
    //{ 
    //     passos++;
    //}
}

bool InputManager::isPressed(uint8_t pin) {
    return !digitalRead(pin);
}