#include "Test.h"
#include <DisplayManager.h>
#include "InputManager.h"
#include "UISystem.h"
#include "ActivityManager.h"
#include "HalConfig.h"
#include <Arduino.h>

void Test::onConfig() {
}

void Test::onPlay() {
}

void Test::onAudioConfig() {
}

void Test::onBack() {
}

Test::Test() 
    : Activity("test", nullptr), mainMenu(nullptr), configMenu(nullptr), lastBtnA(false), lastBtnB(false) {}

void Test::setup() {
    lastBtnA = false;
    lastBtnB = false;
    
    mainMenu = new SimpleUIList(10, 80, 20, 100, 40);
    mainMenu->addOption("Config", onConfig);
    mainMenu->addOption("Play", onPlay);
    pushUIList(mainMenu);
    
    Serial.println("Test setup complete - Main Menu");
}

void Test::loop() {
    
    if (_input) {
        _input->update();
    }
    
    if (_input && _uiSystem) {
        bool currentBtnA = _input->isPressed(BTN_A);
        bool currentBtnB = _input->isPressed(BTN_B);
        Serial.print("BTN_A: ");Serial.println(currentBtnA);
        Serial.print(" | BTN_B: ");Serial.println(currentBtnB);
        if (currentBtnA && !lastBtnA) {
            _uiSystem->handleNextButton();
            
            UIList* topList = _uiSystem->getTopUIList();
            if (topList == mainMenu && mainMenu->getCurOptionIndex() == 0) {
                if (!configMenu) {
                    configMenu = new SimpleUIList(10, 80, 20, 100, 40);
                    configMenu->addOption("Audio", onAudioConfig);
                    configMenu->addOption("Back", onBack);
                }
            }
            
            delay(100);
        }
        lastBtnA = currentBtnA;
        
        if (currentBtnB && !lastBtnB) {
            UIList* topList = _uiSystem->getTopUIList();
            
            if (topList == configMenu && configMenu->getCurOptionIndex() == 1) {
                popUIList();
            }
            else if (topList == mainMenu && mainMenu->getCurOptionIndex() == 0) {
                pushUIList(configMenu);
            }
            else if (topList == mainMenu && mainMenu->getCurOptionIndex() == 1) {
                _uiSystem->handleOKButton();
            }
            
            delay(100);
        }
        lastBtnB = currentBtnB;
    }
    
    _display->clear();
    _activityManager->renderUI();
    
    // Debug: Show button states on screen
    if (_input) {
        _tft->setTextColor(GC9A01A_WHITE);
        _tft->setTextSize(1);
        _tft->setCursor(10, 220);
        _tft->print("B_A: ");
        _tft->println(_input->isPressed(BTN_A) ? "1" : "0");
        
        _tft->setCursor(10, 230);
        _tft->print("B_B: ");
        _tft->println(_input->isPressed(BTN_B) ? "1" : "0");
    }
    
    delay(FRAME_DELAY);
}

void Test::stop() {
    if (mainMenu) {
        popUIList();
        delete mainMenu;
        mainMenu = nullptr;
    }
    if (configMenu) {
        delete configMenu;
        configMenu = nullptr;
    }
    Serial.println("stop");
}

