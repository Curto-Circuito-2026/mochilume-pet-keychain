#ifndef PET_DATA_H
#define PET_DATA_H

#include <Arduino.h>

enum class StatType { ATK, DEF, SPD, HP };
enum class SkillTarget { SELF, OTHER };

struct ShortPetData {
    int specie;
    String name;
    int maxHP;
    int curHP;
    int curSPD;
    int curDEF;
    int curATK;
};

#endif