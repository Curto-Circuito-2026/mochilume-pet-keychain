#include "Activities/Mochilume.h"
#include <DisplayManager.h>
#include <ActivityManager.h>
#include <SaveManager.h>
#include <ArduinoJson.h> 
#include <random>

UIStyle squareHoverStyle = {
    20, 
    20, 
    GC9A01A_WHITE,
    nullptr,
    1,
    0,0,
    0,0,
    GC9A01A_BLACK,
    1
};
UIStyle squareStyle = {
    20, 
    20, 
    GC9A01A_RED,
    nullptr,
    1,
    0,0,
    0,0,
    GC9A01A_BLACK,
    1
};
  UIStyle squareStyle2 = {
    20, 
    20, 
    GC9A01A_BLUE,
    nullptr,
    1,
    0,0,
    0,0,
    GC9A01A_BLACK,
    1
};
  

std::map<int, PetData> petInfoMap = {
  {1, {"test", 0, 0, 0, 0, {{0, {0,1}}}, 0, -1, -1}}  
};

std::map<int, MochilumeSkill> allSkills = {
    {0, {"test", SkillTarget::OTHER, StatType::HP, 10}},
    {1, {"test2", SkillTarget::OTHER, StatType::HP, 10}}
};

void MochilumePet::setData(int species, int level, int xp, String name, int baseHP, int baseDEF, int baseSPD, int baseATK, int skills[4], std::vector<int> skillPoll){
    this->species = species;
    
    this->level = level;
    this->xp = xp;
    this->name = name;

    this->baseHP = baseHP;
    this->curHP = baseHP;

    this->baseDEF = baseDEF;
    this->curDEF = baseDEF;

    this->baseSPD = baseSPD;
    this->curSPD = baseSPD;

    this->baseATK = baseATK;
    this->curATK = baseATK;

    for (int i = 0; i < 4; i++){this->skills[i] = skills[i];}
    this->skillPool.clear();
    for(int s : skillPoll){this->skillPool.push_back(s);}
}

void MochilumePet::changeSkill(int index, int val){
    this->skills[index] = val;
    this->updateSave();
}
void MochilumePet::changeName(String val){
    this->name = val;
    this->updateSave();
}
void MochilumePet::levelUp(){
    this->level += 1;
    this->xp = 0;
    PetData d = petInfoMap[this->species];

    //evolve
    if(this->level == d.evolutionLevel && d.evolutionSpecies > 0){
        this->species = d.evolutionSpecies;
        d = petInfoMap[this->species];
    }
    
    for(int i : d.skillPool[this->level]){
        this->skillPool.push_back(i);
    }

    this->baseHP = d.initialHP + (1.5 * this->level);
    this->baseSPD = d.initialSPD + (1.5 * this->level);
    this->baseATK = d.initialATK + (1.5 * this->level);
    this->baseDEF = d.initialDEF + (1.5 * this->level);

    this->updateSave();
}


void MochilumePet::updateSave(){
    File file = LittleFS.open(String("/mochilume") + "/" + "pets.json", "w");
    if (!file) return;

    JsonDocument doc;
    doc["species"] = this->species;
    doc["level"] = this->level;
    doc["xp"] = this->xp;
    doc["name"] = this->name;
    doc["baseHP"] = this->baseHP;
    doc["baseDEF"] = this->baseDEF;
    doc["baseSPD"] = this->baseSPD;
    doc["baseATK"] = this->baseATK;

    JsonArray skillsArr = doc["skills"].to<JsonArray>();
    for (int i = 0; i < 4; i++) {
        skillsArr.add(this->skills[i]);
    }

    JsonArray poolArr = doc["skillpool"].to<JsonArray>();
    for (int s : this->skillPool) {
        poolArr.add(s);
    }

    doc["skillpoolsize"] = this->skillPool.size();

    serializeJson(doc, file);
    file.close();
}

std::vector<int> MochilumePet::getSkillPool(){
    return this->skillPool;
}


//APP

Mochilume::Mochilume() 
    : Activity("mochilume", nullptr) {}
void Mochilume::setup() {
    this->loadPetData();
    
    this->createHomeScreen();
    this->createStatsScreen();
    this->createBattleSelectionScreen();
    this->createBattleScreen();   

    _screen->changeScreen(home);
}

void Mochilume::createHomeScreen(){
    home = new UIScreen();

    UIElement* exitButton = new UIElement(
        "exitButton", 
        10, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);

    UIElement* statsButton = new UIElement(
        "statsButton", 
        50, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);


    UIElement* battleSelectButton = new UIElement(
        "battleSelectButton", 
        90, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);

    exitButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("SAIR - MENU"); ActivityManager::getInstance()->setActivity("menu"); });

    statsButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR STATS"); _screen->changeScreen(stats); });
    statsButton->setAction(BTN_LEFT, [this](UIElement* element) { Serial.println("TROCA PRO BOTAO 2");  element->getScreen()->setSelectedIndex(1); });

    battleSelectButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR BS"); _screen->changeScreen(battleSelect); });
    battleSelectButton->setAction(BTN_RIGHT, [this](UIElement* element) { Serial.println("TROCA PRO BOTAO 1"); element->getScreen()->setSelectedIndex(0); });

    home->addChild(exitButton);
    home->addChild(statsButton);
    home->addChild(battleSelectButton);

    home->setSelectedIndex(0);

    this->screens["home"] = home;
}

void Mochilume::createStatsScreen(){
    stats = new UIScreen();

    UIElement* exit = new UIElement("exit", 5, 5, squareStyle,  squareHoverStyle, squareStyle);
    exit->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });
    exit->setAction(BTN_RIGHT, [this](UIElement* element) { element->getScreen()->setSelectedIndex(7);  });

    stats->addChild(exit);

    UIElement* name = new UIElement("name", 60, 5, squareStyle, squareHoverStyle, squareStyle);
    name->setText(this->pet->name);
    stats->addChild(name);

    UIElement* xp = new UIElement("xp", 60, 15, squareStyle, squareHoverStyle, squareStyle);
    xp->setText("XP: " + String(this->pet->xp));
    stats->addChild(xp);

    UIElement* skill1 = new UIElement("skill1",5,20,squareStyle, squareHoverStyle, squareStyle);
    skill1->setText(this->pet->skills[0] > 0 ? allSkills[this->pet->skills[0]].name : "S/E");
    stats->addChild(skill1);
    UIElement* skill2 = new UIElement("skill2",25,20,squareStyle, squareHoverStyle, squareStyle);
    skill2->setText(this->pet->skills[1] > 0 ? allSkills[this->pet->skills[1]].name : "S/E");
    stats->addChild(skill2);
    UIElement* skill3 = new UIElement("skill3",5,40,squareStyle, squareHoverStyle, squareStyle);
    skill3->setText(this->pet->skills[2] > 0 ? allSkills[this->pet->skills[2]].name : "S/E");
    stats->addChild(skill3);
    UIElement* skill4 = new UIElement("skill4",25,40,squareStyle, squareHoverStyle, squareStyle);
    skill4->setText(this->pet->skills[3] > 0 ? allSkills[this->pet->skills[3]].name : "S/E");
    stats->addChild(skill4);

    for(int i = 0; i< this->pet->getSkillPool().size(); i++){
        std::string id = "selectSkill" + std::to_string(i);
        int skillId = this->pet->getSkillPool()[i];
       
        UIElement* sB = new UIElement(
            id,
            50,50 + (20*i),
            squareStyle, 
            squareHoverStyle, 
            squareStyle
        );
         for(int s = 0; s<4; s++){
            if(this->pet->skills[s] == skillId){
                sB->setBaseStyle(squareStyle2);
                break;
            }
        }

        exit->setAction(BTN_LEFT, [this](UIElement* element) { element->getScreen()->setSelectedIndex(0);  });

        
        sB->setAction(BTN_UP,[this, i](UIElement* element) {
            int idx = 7 + i - 1;
            if(i <= 0){
                idx = this->pet->getSkillPool().size()-1;
            }
            element->getScreen()->setSelectedIndex(idx); 
        });

        sB->setAction(BTN_DOWN,[this, i](UIElement* element) {
            int idx = 7 + i + 1;
            if(i >= this->pet->getSkillPool().size()-1){
                idx = 0;
            }
            element->getScreen()->setSelectedIndex(idx); 
        });

        sB->setAction(BTN_A, [this, skillId](UIElement* element) { 
            int index = -1;
            int slotIndex = -1;
            for(int i =0; i<4; i++){
                if(this->pet->skills[i] == 0 && slotIndex == -1){slotIndex = i;}
                if(this->pet->skills[i] == skillId){
                index = i;
                break;
            }
            }

            if(index >= 0){
                this->pet->changeSkill(index, 0);
                element->setBaseStyle(squareStyle);
                std::string btnID = "skill" + std::to_string(index+1);
                element->getScreen()->getChild(btnID)->setText("S/E");
                Serial.println("DESEQUIPEI SKILL");
            }else if(slotIndex >= 0){
                this->pet->changeSkill(slotIndex, skillId);
                element->setBaseStyle(squareStyle2);
                std::string btnID = "skill" + std::to_string(slotIndex+1);
                element->getScreen()->getChild(btnID)->setText(allSkills[skillId].name);
                Serial.println("EQUIPEI SKILL");
            }
        });
        stats->addChild(sB);
    }
   


    stats->setSelectedIndex(0);
    this->screens["stats"] = stats;
}

void Mochilume::createBattleSelectionScreen(){
    battleSelect = new UIScreen();

    UIElement* square3 = new UIElement(
        "square3", 
        20, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);
    
    square3->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });
    battleSelect->addChild(square3);
    battleSelect->setSelectedIndex(0);

    this->screens["battleSelect"] = battleSelect;
}

void Mochilume::createBattleScreen(){
    battle = new UIScreen();

    UIElement* square4 = new UIElement(
        "square4", 
        90, 
        20, 
        squareStyle, 
        squareHoverStyle, 
        squareStyle);
    
    battle->addChild(square4);

    this->screens["battle"] = battle;
}

void Mochilume::loadPetData(){
    File file;
    
    int species = 0;
    int level = 1;
    int xp = 0;
    String petName = "";
    int baseHP = 0;
    int baseDEF = 0;
    int baseSPD = 0;
    int baseATK = 0;
    int skills[4] = {0, 0, 0, 0};
    std::vector<int> skillPool;

    bool exists = SaveManager::getInstance()->load("mochilume","pets.json",file);
    if(exists){
        JsonDocument doc;
        deserializeJson(doc, file);

        String n = doc["name"];
        Serial.println(n);

        species = doc["species"].as<int>();
        level = doc["level"].as<int>();
        xp = doc["xp"].as<int>();
        petName = doc["name"].as<String>(); 
        baseHP = doc["baseHP"].as<int>();
        baseDEF = doc["baseDEF"].as<int>();
        baseSPD = doc["baseSPD"].as<int>();
        baseATK = doc["baseATK"].as<int>();
        for(int i = 0; i<4; i++){skills [i] = doc["skills"][i].as<int>();}
        for(int i = 0; i<doc["skillpoolsize"]; i++){skillPool.push_back(doc["skillpool"][i].as<int>());}
    }
    else{
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> distr(1, petInfoMap.size()); 
        species = distr(gen);
        PetData d = petInfoMap[species];
        level = 1;
        xp = 0;
        petName = d.name;
        baseHP = d.initialHP;
        baseDEF = d.initialDEF;
        baseSPD = d.initialSPD;
        baseATK = d.initialATK;
        int c = 0;
        skillPool.clear();
        for(int i : d.skillPool[level]){
            skillPool.push_back(i);
            skills[c] = i;
            c++; 
        }
    }
    if(pet == nullptr){pet = new MochilumePet();}
    pet->setData(species, level, xp, petName, baseHP, baseDEF, baseSPD, baseATK, skills, skillPool);

}

void Mochilume::loop() {
   UIScreen * cs = _screen->getCurScreen();
   if(cs == home){this->homeLoop();}
   else if(cs == stats){this->statsLoop();}
   else if(cs == battleSelect){this->battleSelectionLoop();}
   else if(cs == battle){this->battleLoop();}

    _screen->render();
    
    delay(FRAME_DELAY);
}

void Mochilume::homeLoop(){
    //lora checar se chegou convite pra batalhar
}
void Mochilume::statsLoop(){}
void Mochilume::battleSelectionLoop(){
    //lora buscar gente perto
}
void Mochilume::battleLoop(){
    //logica de combate -> lora enviar pro oponente
}

void Mochilume::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}