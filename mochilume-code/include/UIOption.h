#ifndef UI_OPTION_H
#define UI_OPTION_H

#include <Arduino.h>

typedef void (*UICallback)(void);

class UIOption {
public:
    const char* label;
    UICallback callback;
    
    UIOption(const char* label, UICallback callback = nullptr) 
        : label(label), callback(callback) {}
    
    virtual ~UIOption() {}
    
    void execute() {
        if (callback) {
            callback();
        }
    }
};

#endif
