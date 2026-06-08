#ifndef UISTYLE_H
#define UISTYLE_H
#include <Arduino.h>
#include <Assets.h>
#include <Adafruit_GC9A01A.h>

enum TextAlign {
    LEFT,
    CENTER,
    RIGHT
};


struct UIStyle {
    int16_t width, height;
    int8_t borderRadius;
    uint16_t color;
    bool noFill;
    const Sprite* sprite;
    
    
    int16_t z;

    int16_t offsetX, offsetY;
    int8_t paddingX, paddingY;

    uint16_t textColor;
    uint8_t textSize;
    TextAlign textAlign;
};


//estilos comuns
static const UIStyle field = {
    80,20,
    8,
    COLOR_BACKGROUND_BLUE,
    true,
    nullptr,
    1,
    0,0,
    10,7,
    COLOR_TEXT_MINT,
    1,
    TextAlign::LEFT
};
static const UIStyle hoverField = {
    80,20,
    4,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    1,
    0,0,
    10,7,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::LEFT
};
static const UIStyle button = {
    80,20,
    8,
    COLOR_BACKGROUND_BLUE,
    false,
    nullptr,
    1,
    0,0,
    10,7,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER
};
static const UIStyle hoverButton = {
    80,20,
    8,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    1,
    0,0,
    10,7,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER
};
static const UIStyle emptyStyle = {
    0,0,
    0,
    COLOR_TEXT_MINT,
    true,
    nullptr,
    1,
    0,0,
    0,0,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER
};
static const UIStyle text = {
    0,0,
    0,
    0,
    true,
    nullptr,
    1,
    0,0,
    0,1,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER

};
static const UIStyle leftText = {
    0,0,
    0,
    0,
    true,
    nullptr,
    1,
    0,0,
    0,1,
    COLOR_TEXT_MINT,
    1,
    TextAlign::LEFT

};

static const UIStyle backdrop = {
    240,240,
    0,
    GC9A01A_BLACK,
    false,
    nullptr,
    10,
    0,0,
    0,0,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER
};
static const UIStyle centerText = {
    240,0,
    0,
    0,
    true,
    nullptr,
    0,
    0,0,
    0,0,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER,
};


#endif

