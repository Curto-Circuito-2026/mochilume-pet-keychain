#include <Arduino.h>
#include "HalConfig.h"
#include "DisplayManager.h"

DisplayManager* display;

void setup() {
    Serial.begin(115200);
    delay(2000); //delay pra dar tempo do platformio abrir o serial e n perder os prints
    display = DisplayManager::getInstance();
    display->begin();
    Serial.println("Mochilume OS: Setup Finalizado.");
}

int squareSize = 60;
void loop() {
    static float y = 0;
    float oldY = y;
    
    
    y += 5.0f; 
    if (y >= SCREEN_HEIGHT + squareSize) y = -squareSize;

    Adafruit_GC9A01A* tft = display->getTFT();

    tft->fillRect(90, (int)oldY, squareSize, squareSize, GC9A01A_BLACK);
    //display->clear();
    tft->fillRect(90, (int)y, squareSize, squareSize, GC9A01A_RED);

    delay(FRAME_DELAY);
}