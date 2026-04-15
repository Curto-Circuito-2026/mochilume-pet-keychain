#include "SaveManager.h"

SaveManager* SaveManager::_instance = nullptr;

SaveManager::SaveManager() {}

SaveManager* SaveManager::getInstance() {
    if (_instance == nullptr) _instance = new SaveManager();
    return _instance;
}

//CHECK ARQUIVOS CORROMPIDOS
bool SaveManager::begin() {
    if (!LittleFS.begin(true)) { 
        Serial.println("Erro ao montar LittleFS");
        return false;
    }
    return true;
}