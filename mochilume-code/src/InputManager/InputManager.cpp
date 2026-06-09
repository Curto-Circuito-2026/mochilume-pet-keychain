#include "InputManager.h"
#include "HalConfig.h"
#include <cmath>
#include <algorithm>
#include <Wire.h>

const int MPU_ADDR = 0x68; 

uint8_t buttons[] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_A, BTN_B};

InputManager* InputManager::_instance = nullptr;

InputManager* InputManager::getInstance() {
    if (_instance == nullptr) _instance = new InputManager();
    return _instance;
}

bool InputManager::begin() {
    bateryLevel = 0;
    passosValidados = 0;
    passosCandidatos = 0;
    tempoUltimoPasso = 0;
    estadoAtual = 0; 
    indiceFiltro = 0;
    mpuInitialized = true;
    for (int i = 0; i < AMOSTRAS_FILTRO; i++) {
        historicoMagnitude[i] = 9.81f;
    }

    for(uint8_t btn : buttons) {
        pinMode(btn, INPUT_PULLDOWN);
    }
    
    Wire.begin(MPU_SDA, MPU_SCL);
    
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B); 
    Wire.write(0);    
    if (Wire.endTransmission() != 0) {
        Serial.println("Falha ao encontrar MPU6050");
        mpuInitialized = false;
        return false;
    }
    
    Serial.println("MPU iniciado");
    return true;
}

void InputManager::readIMU() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);
    
    int16_t rawAX = (Wire.read() << 8) | Wire.read();
    int16_t rawAY = (Wire.read() << 8) | Wire.read();
    int16_t rawAZ = (Wire.read() << 8) | Wire.read();
    
    Wire.read(); Wire.read();
    
    int16_t rawGX = (Wire.read() << 8) | Wire.read();
    int16_t rawGY = (Wire.read() << 8) | Wire.read();
    int16_t rawGZ = (Wire.read() << 8) | Wire.read();

    _mpuData.accX = ((float)rawAX / 16384.0f) * 9.80665f;
    _mpuData.accY = ((float)rawAY / 16384.0f) * 9.80665f;
    _mpuData.accZ = ((float)rawAZ / 16384.0f) * 9.80665f;

    _mpuData.gyroX = (float)rawGX / 131.0f;
    _mpuData.gyroY = (float)rawGY / 131.0f;
    _mpuData.gyroZ = (float)rawGZ / 131.0f;
}


void InputManager::processPedometer() {
    float magnitudeAtual = sqrt((_mpuData.accX * _mpuData.accX) + 
                                (_mpuData.accY * _mpuData.accY) + 
                                (_mpuData.accZ * _mpuData.accZ));

    historicoMagnitude[indiceFiltro] = magnitudeAtual;
    indiceFiltro = (indiceFiltro + 1) % AMOSTRAS_FILTRO;

    float soma = 0;
    for (int i = 0; i < AMOSTRAS_FILTRO; i++) {
        soma += historicoMagnitude[i];
    }
    float mediaDinamica = soma / AMOSTRAS_FILTRO;

    unsigned long tempoAtual = millis();
    
    const float SENSIBILIDADE = 0.40f; 
    const int INTERVALO_MIN_PASSO = 350;
    const int INTERVALO_MAX_PASSO = 1500;
    const int MIN_PASSOS_CONSECUTIVOS = 3;
    
    const int STEPS_BATCH_SIZE = 10; 

    if (estadoAtual == 0) { 
        if (magnitudeAtual > (mediaDinamica + SENSIBILIDADE)) {
            estadoAtual = 1; 
            
            long deltaTempo = tempoAtual - tempoUltimoPasso;

            if (deltaTempo >= INTERVALO_MIN_PASSO && deltaTempo <= INTERVALO_MAX_PASSO) {
                passosCandidatos++;

                if (passosCandidatos >= MIN_PASSOS_CONSECUTIVOS) {
                    if (passosValidados == 0) {
                        passosValidados += passosCandidatos;
                        passosLocaisBatch += passosCandidatos;
                    } else {
                        passosValidados++;
                        passosLocaisBatch++;
                    }
                    
                    Serial.print("Passo detectado! Total: ");
                    Serial.print(passosValidados);
                    Serial.print(" | No lote atual: ");
                    Serial.println(passosLocaisBatch);

                    if (passosLocaisBatch >= STEPS_BATCH_SIZE) {
                        //saveSteps(passosLocaisBatch);
                        //passosLocaisBatch = 0; 
                    }
                }
            } else if (deltaTempo > INTERVALO_MAX_PASSO) {
                passosCandidatos = 1; 
            }
            
            tempoUltimoPasso = tempoAtual;
        }
    } else if (estadoAtual == 1) { 
        if (magnitudeAtual < (mediaDinamica - 0.10f)) {
            estadoAtual = 0; 
        }
    }

    if (tempoAtual - tempoUltimoPasso > 2500 && passosCandidatos > 0) {
        passosCandidatos = 0;
    }
}

void InputManager::updateBatteryLevel() {
    int pinRead = analogRead(BATTERY_PIN);
    float vBat = (pinRead * 3.3 / 4095.0) * 2.0;
    if (vBat > 4.2) vBat = 4.2;
    if (vBat < 3.2) vBat = 3.2;  
    int percentage = (int)((vBat - 3.2) * 100.0 / (4.2 - 3.2));
    bateryLevel = percentage;
}


void InputManager::update() {
    _btnStates[0] = digitalRead(BTN_UP);
    _btnStates[1] = digitalRead(BTN_DOWN);
    _btnStates[2] = digitalRead(BTN_LEFT);
    _btnStates[3] = digitalRead(BTN_RIGHT);
    _btnStates[4] = digitalRead(BTN_A);
    _btnStates[5] = digitalRead(BTN_B);
    updateBatteryLevel();
    for (int i = 0; i < 6; i++) {
        if(_btnStates[i] == true) {
            bool found = (std::find(this->inputQueue.begin(), this->inputQueue.end(), buttons[i]) != this->inputQueue.end());
            if(!found) { 
                this->inputQueue.push_back(buttons[i]); 
            }
        }
    }
    
    if(mpuInitialized) {
        readIMU();
        processPedometer();
    }
    
}

bool InputManager::isPressed(uint8_t pin) {
    return digitalRead(pin);
}


void InputManager::setSteps(int quantidadeSalvar) {
    passosValidados = quantidadeSalvar;
}

unsigned long InputManager::getSteps() {
    return passosValidados;
}