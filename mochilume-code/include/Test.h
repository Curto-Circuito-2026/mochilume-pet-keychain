#ifndef TEST_H
#define TEST_H
#include "Activity.h"
#include "SimpleUIList.h"
#include <Adafruit_GC9A01A.h>

class Test : public Activity {
private:
    SimpleUIList* mainMenu;
    SimpleUIList* configMenu;
    bool lastBtnA = false;
    bool lastBtnB = false;
    
    static void onConfig();
    static void onPlay();
    static void onAudioConfig();
    static void onBack();

public:
    Test();

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif