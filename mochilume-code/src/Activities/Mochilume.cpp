#include "Activities/Mochilume.h"
#include <DisplayManager.h>
#include <ActivityManager.h>
#include <SaveManager.h>
#include <ArduinoJson.h> 
#include <random>
#include <UI/UIMenu.h>
#include <UI/UIInput.h>

UIStyle skillButton = {
    60,10,
    4,
    COLOR_APP_GRAY,
    false,
    nullptr,
    1,
    0,0,
    0,1,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER
};
UIStyle hoverSkillButton = {
    60,10,
    4,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    1,
    0,0,
    0,1,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER
};
UIStyle selectedSkillButton = {
    60,10,
    4,
    COLOR_BACKGROUND_BLUE,
    false,
    nullptr,
    1,
    0,0,
    0,1,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER
};


UIStyle resolveText = {
    240,0,
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

UIStyle petImageStyle = {
    96,106,
    0,
    0,
    false,
    &pet_1,
    1,
    0,0,
    0,1,
    COLOR_TEXT_MINT,
    1,
    TextAlign::CENTER
};

UIStyle petText = {
    96,0,
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
UIStyle selectedText = {
    40,10,
    0,
    COLOR_TEXT_MINT,
    false,
    nullptr,
    1,
    0,0,
    0,1,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER

};


UIStyle battleBox = {
    240,55,
    0,
    GC9A01A_WHITE,
    false,
    nullptr,
    1,
    0,0,
    0,0,
    COLOR_BACKGROUND_BLUE,
    1,
    TextAlign::CENTER
};

UIStyle squareHoverStyle = {
    20, 
    20, 
    0,
    GC9A01A_WHITE,
    false,
    nullptr,
    1,
    0,0,
    0,0,
    GC9A01A_BLACK,
    1
};
UIStyle squareStyle = {
    80, 
    20, 
    0,
    GC9A01A_RED,
    false,
    nullptr,
    1,
    0,0,
    10,7,
    GC9A01A_BLACK,
    1
};

std::map<int, PetData> petInfoMap = {
  {1, {"testPET", 0, 0, 0, 0, {{1, {1,2,3,4,5,6,7,8,9,10}}}, &pet_1, -1, -1}}  
};

std::map<int, MochilumeSkill> allSkills = {
    {1, {"test0", SkillTarget::OTHER, StatType::HP, 10}},
    {2, {"test1", SkillTarget::OTHER, StatType::HP, 10}},
    {3, {"test2", SkillTarget::OTHER, StatType::HP, 10}},
    {4, {"test3", SkillTarget::OTHER, StatType::HP, 10}},
    {5, {"test4", SkillTarget::OTHER, StatType::HP, 10}},
    {6, {"test5", SkillTarget::OTHER, StatType::HP, 10}},
    {7, {"test6", SkillTarget::OTHER, StatType::HP, 10}},
    {8, {"test7", SkillTarget::OTHER, StatType::HP, 10}},
    {9, {"test8", SkillTarget::OTHER, StatType::HP, 10}},
    {10, {"test9", SkillTarget::OTHER, StatType::HP, 10}},
};

int MochilumePet::getBaseHP(){
    return this->baseHP;
}
int MochilumePet::getSpecie(){
    return this->species;
}

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
    const char* filePath = "/mochilume/pets.json";
    File file = LittleFS.open(filePath, "w");
    if (!file) {
        Serial.println(F("Failed to open pets.json for writing"));
        return;
    }
    JsonDocument doc;
    doc["species"] = this->species;
    doc["level"]   = this->level;
    doc["xp"]      = this->xp;
    doc["name"]    = this->name;

    JsonArray skillsArr = doc["skills"].to<JsonArray>(); 
    for (int i = 0; i < 4; i++) {
        skillsArr.add(this->skills[i]);
    }
   
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write JSON to pets.json"));
    } else {
        Serial.println(F("Pet data successfully saved!"));
    }

    file.close();
}

std::vector<int> MochilumePet::getSkillPool(){
    return this->skillPool;
}


//APP

Mochilume::Mochilume() 
    : Activity("mochilume", nullptr) {}
void Mochilume::setup() {
    this->loadBaseData();

    this->pet = nullptr;
    this->loadPetData();
    
    this->createHomeScreen();
    this->createStatsScreen();
    this->createBattleSelectionScreen();
    this->createBattleScreen();   

    _screen->changeScreen(home);
}

void Mochilume::createHomeScreen(){
    this->home = new UIScreen();
    // this->home->backgroundImage = bgMainSPR;

    UIElement* statsButton = new UIElement(
        "statsButton", 
        0, 
        0, 
        button, 
        hoverButton, 
        button);
    statsButton->setText("Status");

    UIElement* battleSelectButton = new UIElement(
        "battleSelectButton", 
        0, 
        30, 
        button, 
        hoverButton, 
        button);
    
    battleSelectButton->setText("Batalhar");

     UIElement* exitButton = new UIElement(
        "exitButton", 
        0, 
        60, 
        button, 
        hoverButton, 
        button);
    exitButton->setText("Sair");

    exitButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("SAIR - MENU"); ActivityManager::getInstance()->setActivity("menu"); });
    statsButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR STATS"); _screen->changeScreen(stats); });
    battleSelectButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR BS"); 
        //_screen->changeScreen(battle); 
        this->startBattle(false, "", {});
    });

    UIMenu* homeMenu = new UIMenu(
        "menu",
        80,
        20,
        emptyStyle,
        emptyStyle,
        emptyStyle,
        1,
        0,
        0,
        false
    );

    homeMenu->addChild(statsButton);
    homeMenu->addChild(battleSelectButton);
    homeMenu->addChild(exitButton);
    homeMenu->setSelectedIndex(0);

    home->addChild(homeMenu);
    home->setSelectedIndex(0);
    homeMenu->setState(UIState::SELECTED);

    this->screens["home"] = home;
}

void Mochilume::createStatsScreen(){
    this->stats = new UIScreen();

    UIElement* exit = new UIElement("exit", 80, 200, button, hoverButton, button);
    exit->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });
    stats->addChild(exit);
    exit->setText("Voltar"); 
    
    if(this->pet != nullptr){
        exit->setAction(BTN_RIGHT, [this](UIElement* element) { 
            element->getScreen()->setSelectedIndex(1);
            element->getScreen()->getChild("menu")->setState(UIState::SELECTED);
        });
        
        UIMenu* skillMenu = new UIMenu(
        "menu", 
        150, 60, 
        emptyStyle,
        emptyStyle,
        emptyStyle,
        1,
        0,
        0,
        true);

        skillMenu->setAction(BTN_LEFT, [this](UIElement* element) { element->getScreen()->setSelectedIndex(0); element->setState(UIState::BASE);});
        skillMenu->setAction(BTN_RIGHT, [this](UIElement* element) { element->getScreen()->setSelectedIndex(0); element->setState(UIState::BASE);});

        UIElement* name = new UIElement("name", 120, 5, text, selectedText, text);
        name->setText(this->pet->name);

        exit->setAction(BTN_UP, [this](UIElement* element) {element->getScreen()->setSelectedIndex(2);});

        name->setAction(BTN_DOWN, [this](UIElement* element) {element->getScreen()->setSelectedIndex(0);});
        name->setAction(BTN_RIGHT, [this](UIElement* element) { 
            element->getScreen()->setSelectedIndex(1);
            element->getScreen()->getChild("menu")->setState(UIState::SELECTED);
        });
        name->setAction(BTN_A, [this](UIElement* element) {
            element->getScreen()->getChild("input")->setVisibility(true);
            element->getScreen()->setSelectedIndex(3);
            element->getScreen()->getChild("input")->setSelectedIndex(0);
        });


        UIElement* inputContainer = new UIElement("input", 0, 0, backdrop,backdrop,backdrop);
        UIInput* nameInput = new UIInput("nameInput", 40, 105, 10, this->pet->name);
        inputContainer->addChild(nameInput);
        inputContainer->setAction(BTN_B, [this, nameInput](UIElement* element) {
            element->getScreen()->getChild("input")->setVisibility(false);
            element->getScreen()->setSelectedIndex(2);
            nameInput->setValue(this->pet->name);
        });
        inputContainer->setAction(BTN_A, [this, nameInput](UIElement* element) {
            element->getScreen()->getChild("input")->setVisibility(false);
            element->getScreen()->setSelectedIndex(2);
            this->pet->changeName(nameInput->getValue());
        });

        for(int i = 0; i< this->pet->getSkillPool().size(); i++){
            std::string id = "skill" + std::to_string(i);
            Serial.println(id.c_str());
            int skillId = this->pet->getSkillPool()[i];
        
            UIElement* sB = new UIElement(
                id,
                0,(11*i),
                skillButton, 
                hoverSkillButton, 
                selectedSkillButton
            );
            sB->setText(allSkills[skillId].name);
            for(int s = 0; s<4; s++){
                if(this->pet->skills[s] == skillId){
                    sB->setBaseStyle(selectedSkillButton);
                    break;
                }
            }

            
            sB->setAction(BTN_A, [this, skillId](UIElement* element) { 
                int index = -1;
                int slotIndex = -1;

                for(int i = 0; i < 4; i++){
                    if(this->pet->skills[i] == 0 && slotIndex == -1) { slotIndex = i; }
                    if(this->pet->skills[i] == skillId){
                        index = i;
                        break;
                    }
                }       
                

                if(index >= 0){
                    this->pet->changeSkill(index, 0);
                    element->setBaseStyle(skillButton);
                    std::string btnID = "skillSlot" + std::to_string(index + 1);
                        UIElement* child = this->stats->getChild(btnID);
                        if (child != nullptr) {
                            Serial.println("oi");
                            child->setText("S/E");
                        } else {
                            Serial.printf("CRASH PREVENTED: Child UI element '%s' not found!\n", btnID.c_str());
                        }
                    
                    Serial.println("DESEQUIPEI SKILL");
                }else if(slotIndex >= 0){
                    this->pet->changeSkill(slotIndex, skillId);
                    element->setBaseStyle(selectedSkillButton);
                    std::string btnID = "skillSlot" + std::to_string(slotIndex + 1);

                        UIElement* child = this->stats->getChild(btnID);
                        if (child != nullptr) {
                            Serial.println("oi");
                            child->setText(allSkills[skillId].name);
                        } else {
                            Serial.printf("CRASH PREVENTED: Child UI element '%s' not found!\n", btnID.c_str());
                        }
                    
                    Serial.println("EQUIPEI SKILL");
                }
            });
            skillMenu->addChild(sB);
        }
        
        skillMenu->setSelectedIndex(0);

        stats->addChild(skillMenu);
        stats->setSelectedIndex(0);

        stats->addChild(name);
        
        stats->addChild(inputContainer);
        inputContainer->setVisibility(false);

        UIElement* level = new UIElement("level", 120, 15, text, text, text);
        level->setText("LV: " + String(this->pet->level));
        stats->addChild(level);

        UIElement* xp = new UIElement("xp", 120, 25, text, text, text);
        xp->setText("XP: " + String(this->pet->xp) + "/100");
        stats->addChild(xp);

        UIElement* skLabel = new UIElement("skLabel", 80, 50, text, text, text);
        skLabel->setText("Skills Equipadas:");
        stats->addChild(skLabel);

        UIElement* petImage = new UIElement("petImage", 30, 80, petImageStyle, petImageStyle, petImageStyle);
        stats->addChild(petImage);

        UIElement* skill1 = new UIElement("skillSlot1",20,60,selectedSkillButton, selectedSkillButton, selectedSkillButton);
        skill1->setText(this->pet->skills[0] > 0 ? allSkills[this->pet->skills[0]].name : "S/E");
        stats->addChild(skill1);
        UIElement* skill2 = new UIElement("skillSlot2",81,60,selectedSkillButton, selectedSkillButton, selectedSkillButton);
        skill2->setText(this->pet->skills[1] > 0 ? allSkills[this->pet->skills[1]].name : "S/E");
        stats->addChild(skill2);
        UIElement* skill3 = new UIElement("skillSlot3",20,71,selectedSkillButton, selectedSkillButton, selectedSkillButton);
        skill3->setText(this->pet->skills[2] > 0 ? allSkills[this->pet->skills[2]].name : "S/E");
        stats->addChild(skill3);
        UIElement* skill4 = new UIElement("skillSlot4",81,71,selectedSkillButton, selectedSkillButton, selectedSkillButton);
        skill4->setText(this->pet->skills[3] > 0 ? allSkills[this->pet->skills[3]].name : "S/E");
        stats->addChild(skill4);

        

    }
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
    //player
    UIElement* playerSquare = new UIElement(
        "player", 
        0, 
        185, 
        battleBox, 
        battleBox, 
        battleBox);

    UIMenu* skillMenu = new UIMenu(
        "skills",
        60,
        210,
        text,text,text,
        2,0,0
    );
    for(int i = 0; i< 4; i++){
        int sk = this->pet->skills[i];
        std::string id = "selectSkill" + std::to_string(i);
        
        UIElement* sb = new UIElement(
            id,
            i%2 == 0 ? 0 : 61,
            i<2? 0 : 11,
            selectedSkillButton, 
            hoverSkillButton, 
            selectedSkillButton
        );

        sb->setText(allSkills[sk].name);

        sb->setAction(BTN_A, [this, sk](UIElement* element){
            if(this->battleInfo.status == BattleStatus::PlayerTurn){
                this->battleInfo.selectedSkill = sk;
                if(this->battleInfo.enemySkill == -1){
                    this->battleInfo.status = BattleStatus::WaitingEnemy;
                    this->battle->getChild("resolve")->setText("Aguardando Oponente...");
                }else{
                    this->resolveBattleTurn();
                }
            }
        });
        skillMenu->addChild(sb);
    }
    playerSquare->addChild(skillMenu);

    
    
    battle->addChild(playerSquare);
    UIElement* petImage = new UIElement("image", 10, -80, petImageStyle, petImageStyle, petImageStyle);
    UIElement* petName = new UIElement("name", 10, -100, petText,petText,petText);
    petName->setText(this->pet->name);
    UIElement* petHP = new UIElement("hp", 10, -90, petText,petText,petText);
    petHP->setText(String(this->pet->curHP) + "/" + String(this->pet->getBaseHP()));

    playerSquare->addChild(petImage);
    playerSquare->addChild(petName);
    playerSquare->addChild(petHP);

    //resolve
    UIElement* resolve = new UIElement(
        "resolve", 
        0, 
        35, 
        resolveText, 
        resolveText, 
        resolveText);
    
    battle->addChild(resolve);
    resolve->setText("");

    //enemy
    UIElement* enemySquare = new UIElement(
        "enemy", 
        0, 
        0, 
        text, 
        text, 
        text);

    UIElement* enemyImage = new UIElement("image", 150, 80, petImageStyle, petImageStyle, petImageStyle);
    UIElement* enemyName = new UIElement("name", 150, 60, petText,petText,petText);
    enemyName->setText("inimigo");
    UIElement* enemyHP = new UIElement("hp", 150, 70, petText,petText,petText);
    enemyHP->setText("0/100");

    enemySquare->addChild(enemyImage);
    enemySquare->addChild(enemyName);
    enemySquare->addChild(enemyHP);
    
    battle->addChild(enemySquare);

    battle->setSelectedIndex(0);
    playerSquare->setSelectedIndex(0);
    skillMenu->setState(UIState::SELECTED);

    this->screens["battle"] = battle;
}

void Mochilume::startBattle(bool host, String seed, ShortPetData enemy){
    this->battle->getChild("resolve")->setText("Escolha uma skill");
    this->battleInfo.enemySkill = -1;
    this->battleInfo.selectedSkill = -1;
    this->battleInfo.status = BattleStatus::PlayerTurn;
    this->battleInfo.isHost = host;
    this->battleInfo.seed = seed;

    UIElement* enemySQ = this->battle->getChild("enemy");
    enemySQ->getChild("name")->setText(enemy.name);
    enemySQ->getChild("hp")->setText(String(enemy.curHP) + "/" + String(enemy.maxHP));
    enemySQ->getChild("image")->setBaseStyle({
        96,106,
        0,
        0,
        false,
        petInfoMap[enemy.specie].sprite,
        1,
        0,0,
        0,1,
        COLOR_TEXT_MINT,
        1,
        TextAlign::CENTER
    });

    UIElement* playerSQ = this->battle->getChild("player");
    playerSQ->getChild("name")->setText(this->pet->name);
    playerSQ->getChild("hp")->setText(String(this->pet->curHP) + "/" + String(this->pet->getBaseHP()));
    playerSQ->getChild("image")->setBaseStyle({
        96,106,
        0,
        0,
        false,
        petInfoMap[this->pet->getSpecie()].sprite,
        1,
        0,0,
        0,1,
        COLOR_TEXT_MINT,
        1,
        TextAlign::CENTER
    });

    this->_screen->changeScreen(battle);

}
void Mochilume::resolveBattleTurn(){
    this->battle->getChild("resolve")->setText("");
};

void Mochilume::loadBaseData(){
  
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

    String filePath = "/mochilume/pets.json";
    bool fileLoadedSuccessfully = false;

    if (LittleFS.exists(filePath)) {
        File file = LittleFS.open(filePath, "r");
        if (file) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, file);
            
            file.close(); 

            if (!error) {
                fileLoadedSuccessfully = true;

                String n = doc["name"];
                Serial.println(n);

                species = doc["species"].as<int>();
                level = doc["level"].as<int>();
                xp = doc["xp"].as<int>();
                petName = doc["name"].as<String>();
                
                JsonArray skillsArray = doc["skills"];
                for (int i = 0; i < 4 && i < skillsArray.size(); i++) {
                    skills[i] = skillsArray[i].as<int>();
                }

                PetData d = petInfoMap[species];

                baseHP = d.initialHP + (1.5 * level);
                baseDEF = d.initialDEF + (1.5 * level);
                baseSPD = d.initialSPD + (1.5 * level);
                baseATK = d.initialATK + (1.5 * level);

                for(int i = 1; i <= level; i++){
                    skillPool.reserve(skillPool.size() + d.skillPool[i].size());
                    skillPool.insert(skillPool.end(), d.skillPool[i].begin(), d.skillPool[i].end());
                }
            } else {
                Serial.print("JSON Parsing failed: ");
                Serial.println(error.f_str());
            }
        }
    }

    if (!fileLoadedSuccessfully) {
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
            if(c < 4){
                skills[c] = i;
                c++; 
            }
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