#ifndef TEST_H
#define TEST_H
#include "Activity.h"
#include <Adafruit_GC9A01A.h>

class Test : public Activity {
private:
    float y;
    int squareSize;
    Adafruit_GC9A01A* tft;

public:
    Test();

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif