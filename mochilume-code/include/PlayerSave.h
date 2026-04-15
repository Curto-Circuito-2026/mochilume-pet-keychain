#ifndef SAVE_DEFS_H
#define SAVE_DEFS_H

#include <Arduino.h>

struct PlayerSave{
    //PROGRESSO]
    char name [20];
    uint8_t level;
    uint16_t experience;
    uint32_t stepsTotal;
    uint32_t money;

    //CONFIGURAÇÕES

    bool soundEnabled;
    uint8_t volume;

    //CHECKSUM DE SEGURANCA PARA ARQUIVO CORROMPIDO
    uint32_t checksum;
};

#endif