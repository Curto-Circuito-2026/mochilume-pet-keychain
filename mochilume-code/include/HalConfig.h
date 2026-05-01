#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#include <Arduino.h>

// --- DISPLAY SETTINGS (GC9A01) ---
#define TFT_RES      4
#define TFT_CS       5
#define TFT_DC       15
#define TFT_MOSI     23
#define TFT_SCLK     18
#define TFT_BL       22

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240
#define SCREEN_RADIUS 120
#define SCREEN_CENTER 120

// --- BUTTONS ---
//definir pinos reais
#define BTN_UP       39
#define BTN_DOWN     38
#define BTN_LEFT     37
#define BTN_RIGHT    36
#define BTN_A        27
#define BTN_B        26

// --- SYSTEM SETTINGS ---
#define TARGET_FPS   60
#define FRAME_DELAY  (1000 / TARGET_FPS)

// --- LORA SX1268 (SPI) ---
#define LORA_CS      14
#define LORA_DIO1    26
#define LORA_RST     12
#define LORA_BUSY    13

// --- STORAGE (SD CARD) ---
#define SD_CS        2

// --- SENSORS (I2C) ---
#define MPU_SDA      21
#define MPU_SCL      22


//PATH DE ARQUIVO DE SAVE
#define PLAYER_SAVE_PATH "/player.sav"


#endif