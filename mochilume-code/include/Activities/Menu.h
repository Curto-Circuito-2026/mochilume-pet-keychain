#ifndef MENU_H
#define MENU_H
#include "Activity.h"
#include <Adafruit_GC9A01A.h>

class Menu : public Activity {
private:
    
public:
    Menu();

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif