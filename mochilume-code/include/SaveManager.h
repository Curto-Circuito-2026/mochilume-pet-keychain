#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>

class SaveManager {
private:
    static SaveManager* _instance;
    SaveManager();

public:
    static SaveManager* getInstance();
    
    bool begin(); 

    template <typename T>
    bool save(const char* path, const T& data) {
        File file = LittleFS.open(path, "w");
        if (!file) return false;
        
        size_t written = file.write((const uint8_t*)&data, sizeof(T));
        file.close();
        return written == sizeof(T);
    }

    template <typename T>
    bool load(const char* path, T& data) {
        if (!LittleFS.exists(path)) return false;

        File file = LittleFS.open(path, "r");
        if (!file) return false;

        size_t read = file.read((uint8_t*)&data, sizeof(T));
        file.close();
        return read == sizeof(T);
    }
};

#endif