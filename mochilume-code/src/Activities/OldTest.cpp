// #include "Test.h"
// #include <DisplayManager.h>
// #include "InputManager.h"
// #include "HalConfig.h"
// #include <Arduino.h>

// Test::Test() 
//     : Activity("test", nullptr) {}

// void Test::setup() {
//     Serial.println("Test setup complete");    
//     _tft->fillRect(30, 120, 50, 50,  GC9A01A_RED);
// }

// void Test::loop() {
//     // Update input manager to read button states
//     InputManager* input = InputManager::getInstance();
//     if (input) {
//         input->update();
//     }
   
//     // Clear display
//     //DisplayManager::getInstance()->clear();
    
//     // Simple test display
//     if (input) {
//         bool btnA = input->isPressed(BTN_A);
//         bool btnB = input->isPressed(BTN_B);
        
//         // Draw large button status
//         _tft->setTextColor(GC9A01A_WHITE);
//         _tft->setTextSize(2);
//         _tft->setCursor(20, 50);
//         _tft->print("BTN_A: ");
//         _tft->println(btnA ? "1" : "0");
        
//         _tft->setCursor(20, 100);
//         _tft->print("BTN_B: ");
//         _tft->println(btnB ? "1" : "0");
        
//         // Color box to visualize button states
//         uint16_t colorA = btnA ? GC9A01A_GREEN : GC9A01A_RED;
//         uint16_t colorB = btnB ? GC9A01A_GREEN : GC9A01A_RED;
        
//         _tft->fillRect(30, 120, 50, 50, colorA);
//         _tft->fillRect(130, 120, 50, 50, colorB);
//     }
    
//     delay(FRAME_DELAY);
// }

// void Test::stop() {
//     Serial.println("stop");
// }