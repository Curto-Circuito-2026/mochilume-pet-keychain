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
    bool save(const char* folder, const char* fileName, const T& data) {
        String folderPath = String("/") + folder;
        String filePath   = folderPath + "/" + fileName;
        if (!LittleFS.exists(folderPath)) {
            bool ok = LittleFS.mkdir(folderPath);
            if (!ok) {
                LittleFS.format();
                LittleFS.begin();
                LittleFS.mkdir(folderPath);
            }
        }

        File file = LittleFS.open(filePath, "w");
        if (!file) return false;
        
        size_t written = file.write((const uint8_t*)&data, sizeof(T));
        file.close();
        return written == sizeof(T);
    }

    template <typename T>
    bool load(const char* folder, const char* fileName, T& data) {
        String folderPath = String("/") + folder;
        String filePath   = folderPath + "/" + fileName;
        if (!LittleFS.exists(filePath)) return false;

        File file = LittleFS.open(filePath, "r");
        if (!file) return false;

        size_t read = file.read((uint8_t*)&data, sizeof(T));
        file.close();
        return read == sizeof(T);
    }
};

#endif