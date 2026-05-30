#ifndef SAVE_DEFS_H
#define SAVE_DEFS_H

#include <Arduino.h>

struct PlayerSave{
    //PROGRESSO]
    char name [20];
    uint32_t stepsTotal;

    //CHECKSUM DE SEGURANCA PARA ARQUIVO CORROMPIDO
    uint32_t checksum;
};

#endif