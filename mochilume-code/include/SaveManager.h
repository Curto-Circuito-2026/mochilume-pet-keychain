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
    bool save(const char* folder, const char* file, const T& data) {
        if (!LittleFS.exists("/"+folder)) {
            bool ok = LittleFS.mkdir("/"+folder);
            if (!ok) {
                LittleFS.format();
                LittleFS.begin();
                LittleFS.mkdir("/"+folder);
            }
        }

        File file = LittleFS.open("/"+folder+"/"+file, "w");
        if (!file) return false;
        
        size_t written = file.write((const uint8_t*)&data, sizeof(T));
        file.close();
        return written == sizeof(T);
    }

    template <typename T>
    bool load(const char* folder, const char* file, T& data) {
        if (!LittleFS.exists("/"+folder+"/"+file)) return false;

        File file = LittleFS.open("/"+folder+"/"+file, "r");
        if (!file) return false;

        size_t read = file.read((uint8_t*)&data, sizeof(T));
        file.close();
        return read == sizeof(T);
    }
};

#endif