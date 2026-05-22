#ifndef MOCHILUME_H
#define MOCHILUME_H
#include "Activity.h"
#include <Adafruit_GC9A01A.h>

enum StatType {
    DEF,
    SPD,
    ATK,
    HP
};
enum SkillTarget {
    SELF,
    OTHER
};

struct MochilumeSkill {
    SkillTarget target;
    StatType stat;
    int value;
};

struct PetData {
    int initialHP;
    int initialDEF;
    int initalSPD;
    int initialATK;

    //skills disponiveis por level
    std::map<int, std::vector<MochilumeSkill>> skillPool;

    const uint8_t* sprite;

    String evolutionSpecies;
    int evolutionLevel;
};

//mapa de infos base do pet por id da especie
extern std::map<String, PetData> petInfoMap; 

class MochilumePet {
    public:
        int level;
        int xp;
        String name;

        int curHP;
        int curDEF;
        int curSPD;
        int curATK;

        MochilumeSkill skills[4];
    private: 
        String species;

        //adiciona do petData cada levelUP e no evolve.
        std::vector<MochilumeSkill> skillPool;

        int baseHP;
        int baseDEF;
        int baseSPD;
        int baseATK;

        void levelUp();
        void evolve();

};

class Mochilume : public Activity {
private:
    UIScreen* home;
    UIScreen* stats;
    UIScreen* battleSelect;
    UIScreen* battle;
public:
    Mochilume();

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif