#ifndef TEST_H
#define TEST_H
#include "Activity.h"
#include "SimpleUIList.h"
#include <Adafruit_GC9A01A.h>

class Test : public Activity {
private:
    float y;
    int16_t squareSize;
    UIElement* square;
    UIElement* square2;
    UIList* mainMenu;
    UIList* configMenu;
    bool lastBtnA;
    bool lastBtnB;

    void onConfig();
    void onPlay();
    void onAudioConfig();
    void onBack();

public:
    Test();

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif