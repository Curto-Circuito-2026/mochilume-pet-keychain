#include "Test.h"
#include <DisplayManager.h>


Test::Test() 
    : Activity("test", nullptr) {}
void Test::setup() {
    y = 0;
    squareSize = 60;
}
void Test::loop() {
    float oldY = y;
    y += 5.0f; 
    if (y >= SCREEN_HEIGHT + squareSize) y = -squareSize;
    _tft->fillRect(90, (int)oldY, squareSize, squareSize, GC9A01A_BLACK);
    _tft->fillRect(90, (int)y, squareSize, squareSize, GC9A01A_RED);
    delay(FRAME_DELAY);
}
void Test::stop() {
    Serial.println("stop");
}