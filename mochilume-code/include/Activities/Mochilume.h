#ifndef MOCHILUME_H
#define MOCHILUME_H
#include "Activity.h"
#include <Adafruit_GC9A01A.h>
#include "PetData.h"
#include "LoraManager.h"
#include "PacketModels.h"

struct MochilumeSkill {
    String name;
    SkillTarget target;
    StatType stat;
    int value;
};

struct PetData {
    String name;
    int initialHP;
    int initialDEF;
    int initialSPD;
    int initialATK;

    //id das skills disponiveis por level
    std::map<int, std::vector<int>> skillPool;

    const Sprite* sprite;

    int evolutionSpecies;
    int evolutionLevel;
};

//mapa de infos base do pet por id da especie
extern std::map<int, PetData> petInfoMap; 
//mapa de skills por id
extern std::map<int, MochilumeSkill> allSkills;

class MochilumePet {
    public:
        int level;
        int xp;
        String name;

        int curHP;
        int curDEF;
        int curSPD;
        int curATK;

        //id das skills
        int skills[4];

        std::vector<int> getSkillPool();

        void setData(int species, int level, int xp, String name, int baseHP, int baseDEF, int baseSPD, int baseATK, int skills[4], std::vector<int> skillPoll);

        //atualizar no save
        void changeName(String val);
        void changeSkill(int index, int val);

        int getBaseHP();
        int getSpecie();

        //usar em batalha
        void changeCurHP(int addr);
        void changeCurDEF(int addr);
        void changeCurSPD(int addr);
        void changeCurATK(int addr);

    private: 
        int species;

        //adiciona id da skil do petData cada levelUP e no evolve.
        std::vector<int> skillPool;

        int baseHP;
        int baseDEF;
        int baseSPD;
        int baseATK;

        void updateSave();
        
        //muda atributos base e skill pool. atualizar no save
        void levelUp();

};



enum BattleStatus {
    PlayerTurn,
    WaitingEnemy,
    Resolve,
    None
};

struct Battle{
    BattleStatus status;
    String seed;
    ShortPetData enemy;
    int selectedSkill;
    int enemySkill;
    bool isHost;
    bool received = false;
    std::vector<BattleAction> actions;
};

class Mochilume : public Activity {
private:
    UIScreen* home;
    UIScreen* stats;
    UIScreen* battleSelect;
    UIScreen* battle;

    MochilumePet* pet;

    Battle battleInfo;
    void startBattle(bool host, String seed, ShortPetData enemy);
    std::vector<BattleAction> resolveBattleTurn();
    void passBattleActions(std::vector<BattleAction> actions);

    void loadPetData();

    void loadBaseData();

    void createHomeScreen();
    void createStatsScreen();
    void createBattleSelectionScreen();
    void createBattleScreen();

    
    void battleSelectionLoop();
    void statsLoop();
    void homeLoop();
    void battleLoop();

    long lastPingTime = 0;
    const long pingInterval = 5000; // 5 segundos
public:
    Mochilume();

    void setup() override;
    void loop() override;
    void stop() override;
};

#endif