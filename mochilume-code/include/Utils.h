#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"
#include <random>

String generateRandomString(int length){
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    String result = "";
    
    for (size_t i = 0; i < length; i++) {
        uint32_t randVal = esp_random(); 
        int index = randVal % (sizeof(charset) - 1);
        result += charset[index];
    }
    return result;
}

#endif