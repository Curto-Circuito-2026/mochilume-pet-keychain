#include "Activities/Menu.h"
#include "ActivityManager.h"
#include <DisplayManager.h>
#include "Assets.h"

Menu::Menu() 
    : Activity("menu", nullptr) {}
void Menu::setup() {

    this->curApp = 0;
    loadActivities();

    UIScreen* screen1 = new UIScreen();
    screen1->backgroundColor = GC9A01A_WHITE;
    
    UIStyle frameStyle = {
        240,
        240,
        0,
        GC9A01A_WHITE,
        false,
        &frame,
        0,
        0,0,
        0,0,
        GC9A01A_BLACK,
        1
    };

    UIStyle textStyle = {
        110,20,
        0,
        0,
        true,
        nullptr,
        0,
        0,0,
        0,0,
        COLOR_TEXT_MINT,
        2,
        TextAlign::CENTER
    };

    UIStyle mainAppStyle = {
        104,104,
        52,
        COLOR_APP_GRAY,
        false,
        activities[curApp]->icon,
        0,
        0,
        0,
        0,
        0,
        GC9A01A_BLACK,
        1
    };

    UIStyle sideAppStyle = {
        52,
        52,
        26,
        COLOR_APP_GRAY,
        false,
        nullptr,
        0,
        0,0,
        0,0,
        GC9A01A_BLACK,
        1
    };

    UIElement* frame = new UIElement(
        "frame",
        0,
        0,
        frameStyle,
        frameStyle,
        frameStyle
    );
    screen1->addChild(frame);

    mainApp = new UIElement(
        "mainApp",
        68,
        68,
        mainAppStyle,
        mainAppStyle,
        mainAppStyle
    );
    frame->addChild(mainApp);
    
    appName = new UIElement(
        "appName",
        65,
        210,
        textStyle,
        textStyle,
        textStyle
    );
    appName->setText(activities[curApp]->name);
    frame->addChild(appName);

    if(activities.size() > 1){
        leftApp = new UIElement(
            "leftApp",
            9,
            94,
            sideAppStyle,
            sideAppStyle,
            sideAppStyle
        );
        rightApp = new UIElement(
            "rightApp",
            179,
            94,
            sideAppStyle,
            sideAppStyle,
            sideAppStyle
        );
        frame->addChild(leftApp);
        frame->addChild(rightApp);
    }


    frame->setAction(BTN_A, [this](UIElement* element){ActivityManager::getInstance()->setActivity(this->activities[this->curApp]->name);});
    frame->setAction(BTN_LEFT, [this](UIElement* element){
        if(this->curApp-1 < 0){this->curApp = this->activities.size()-1;}
        else{this->curApp -= 1;}
        this->appName->setText(this->activities[this->curApp]->name);
        UIStyle nL= {    104,104,
            52,
            COLOR_APP_GRAY,
            false,
            this->activities[this->curApp]->icon,
            0,
            0,
            0,
            0,
            0,
            GC9A01A_BLACK,
            1};
        this->mainApp->setBaseStyle(nL);
        this->mainApp->setHoverStyle(nL);
        this->mainApp->setSelectedStyle(nL);
    });
    frame->setAction(BTN_RIGHT, [this](UIElement* element){
        if(this->curApp+1 >= this->activities.size()){this->curApp = 0;}
        else{this->curApp += 1;}
        this->appName->setText(this->activities[this->curApp]->name);
        UIStyle nL= {    104,104,
            52,
            COLOR_APP_GRAY,
            false,
            this->activities[this->curApp]->icon,
            0,
            0,
            0,
            0,
            0,
            GC9A01A_BLACK,
            1};
        this->mainApp->setBaseStyle(nL);
        this->mainApp->setHoverStyle(nL);
        this->mainApp->setSelectedStyle(nL);
    });

    UIStyle stepIcoStyle = {
        11,13,
        0,
        0,
        true,
        &step_ico,

        0,
        0,0,
        13,4,
        COLOR_TEXT_MINT,
        1,
        TextAlign::LEFT
    };
    UIElement* stepsIco = new UIElement("stepIco",22,50,stepIcoStyle,stepIcoStyle,stepIcoStyle);
    stepsIco->setText("20");

    UIStyle batteryIcoStyle = {
        11,13,
        0,
        0,
        true,
        &battery_ico,

        2,
        0,0,
        13,4,
        COLOR_TEXT_MINT,
        1,
        TextAlign::LEFT
    };
    UIElement* batteryIco = new UIElement("batteryIco",190,50,batteryIcoStyle,batteryIcoStyle,batteryIcoStyle);
    
    screen1->addChild(stepsIco);
    screen1->addChild(batteryIco);
    screen1->setSelectedIndex(0);
    _screen->changeScreen(screen1);
};
void Menu::loadActivities(){
    this->activities = ActivityManager::getInstance()->getActivities();
}



void Menu::loop() {
    _screen->getCurScreen()->getChild("stepIco")->setText(String(InputManager::getInstance()->getSteps()));
    _screen->getCurScreen()->getChild("batteryIco")->setText(String(InputManager::getInstance()->getBatteryLevel()) + "%");
    _screen->render();
    
}
void Menu::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}