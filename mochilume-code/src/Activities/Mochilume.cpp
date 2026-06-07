#include "Activities/Mochilume.h"
#include <DisplayManager.h>
#include <ActivityManager.h>
#include <SaveManager.h>
#include <ArduinoJson.h> 
#include <random>
#include <UI/UIMenu.h>
#include <UI/UIInput.h>
#include <WifiManager.h>

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
        _screen->changeScreen(battleSelect); 
        //this->startBattle(false, "", {});
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

    this->battleSelect = new UIScreen();

  
    UIElement* exitButton = new UIElement(
        "exitButton", 
        0, 
        120, 
        button, 
        hoverButton, 
        button);
    exitButton->setText("Sair");
    exitButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("SAIR - MENU"); _screen->changeScreen(home);});
   

    UIElement* acceptMessage = new UIElement(
        "acceptMessage", 
        0, 
        0, 
        backdrop, 
        backdrop, 
    backdrop);
    UIElement* acceptText = new UIElement(
        "acceptText", 
        90, 
        80, 
        text, 
        text, 
        text);
    acceptText->setText("Aceitar desafio?");

    UIMenu* acceptMenu = new UIMenu(
        "acceptMenu",
        80,
        100,
        emptyStyle,
        emptyStyle,
        emptyStyle,
        1,
        0,
        0,
        false
    );

    UIElement* acceptButton = new UIElement(
        "acceptButton", 
        0, 
        0, 
        button, 
        hoverButton, 
        button);
        acceptButton->setText("Aceitar");
    acceptButton->setAction(BTN_A, [this](UIElement* element) { 
        BattleStartModel model;
        model.isHost = false;
        model.seed = String(random(0, 100000));
        model.opponentPet = {
            .specie = this->pet->getSpecie(),
            .name = this->pet->name,
            .maxHP = this->pet->curHP,
            .curHP = this->pet->curHP,
            .curSPD = this->pet->curSPD,
            .curDEF = this->pet->curDEF,
            .curATK = this->pet->curATK
        };
        LoraManager::getInstance()->sendPacket<BattleStartModel>(model, BATTLE_START);
        startBattle(false, model.seed, battleInfo.enemy);
    });

    UIElement* declineButton = new UIElement(
        "declineButton", 
        0, 
        30, 
        button, 
        hoverButton, 
        button);
    declineButton->setText("Recusar");
    declineButton->setAction(BTN_A, [this, acceptMessage](UIElement* element) {    
        String msg = "BattleDecline";
        LoraManager::getInstance()->sendPacket<String>(msg, MESSAGE);
        acceptMessage->setVisibility(false);
        this->battleSelect->setSelectedIndex(0);
        this->battleSelect->getChild("menu")->setState(UIState::SELECTED);
        LoraManager::getInstance()->disconnect();
    });

    acceptMenu->addChild(acceptButton);
    acceptMenu->addChild(declineButton);
    acceptMessage->addChild(acceptMenu);
    acceptMessage->addChild(acceptText);
    acceptMessage->setSelectedIndex(0);
    acceptMenu->setState(UIState::SELECTED);
    acceptMessage->setVisibility(false);

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
    homeMenu->addChild(exitButton);
    homeMenu->setSelectedIndex(0);
    battleSelect->addChild(homeMenu);
    battleSelect->setSelectedIndex(0);
    battleSelect->addChild(acceptMessage);
    homeMenu->setState(UIState::SELECTED);

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

BattleAction createBattleAction(String casterName, String targetName, MochilumeSkill skill){
    String action = "";
    String result = "";

    String skillName = skill.name;
    action = "O" + casterName + "usou " + skillName;
    if(skill.target == SkillTarget::SELF){
        action += " em si mesmo";
    }else{
        action += " em " + targetName;    
    }
    result += "O " ;
    switch (skill.stat)
    {
        
        case StatType::HP:
        result += "HP";
        break;
        case StatType::ATK:
        result += "ATK";
        break;
        case StatType::DEF:
        result += "DEF";
        break;
        case StatType::SPD:
        result += "SPD";
        break;
         if(skill.target == SkillTarget::SELF){
            result += "Do " + casterName + " aumentou" ;
        }else{
            result += "Do " + targetName + " foi reduzido" ;
        }
        result += " em " +String(skill.value);
    }
    int value = skill.value;

    return BattleAction{
        .action = action,
        .result = result,
        .value = value,
        .stat = skill.stat,
        .target = skill.target
    };
}

std::vector<BattleAction> Mochilume::resolveBattleTurn(){
    battleInfo.status = BattleStatus::Resolve;
    std::vector<BattleAction> actions;

    BattleAction enemyAction = createBattleAction(battleInfo.enemy.name, this->pet->name, allSkills[this->battleInfo.enemySkill]);
    BattleAction yourAction = createBattleAction(this->pet->name, battleInfo.enemy.name, allSkills[this->battleInfo.selectedSkill]);
    yourAction.isHost = battleInfo.isHost;
    enemyAction.isHost = !battleInfo.isHost;

    if(battleInfo.enemy.curSPD > this->pet->curSPD){
       actions.push_back(enemyAction);   
       actions.push_back(yourAction);
    }else{
        actions.push_back(yourAction);
        actions.push_back(enemyAction);
    }

    battleInfo.selectedSkill = -1;
    battleInfo.enemySkill = -1;
    
    battleInfo.actions = actions;
    BattleTurnModel model;
    model.actions = battleInfo.actions;
    LoraManager::getInstance()->sendPacket<BattleTurnModel>(model, BATTLE_TURN);
    return actions;
};

void Mochilume::passBattleActions(std::vector<BattleAction> actions){
    for(BattleAction action : actions){
        this->battle->getChild("resolve")->setText(action.action);
        delay(300);
        this->battle->getChild("resolve")->setText(action.result);

        switch (action.stat)
        {
            case StatType::HP:
                if(action.target == SkillTarget::SELF){
                    if(action.isHost == battleInfo.isHost){
                        this->pet->curHP += action.value;
                    }else{
                        this->battleInfo.enemy.curHP += action.value;
                    }
                }else{
                    if(action.isHost == battleInfo.isHost){
                        this->battleInfo.enemy.curHP -= action.value;
                    }else{
                        this->pet->curHP -= action.value;
                    }
                }
                this->battle->getChild("player")->getChild("hp")->setText(String(this->pet->curHP) + "/" + String(this->pet->getBaseHP()));
                this->battle->getChild("enemy")->getChild("hp")->setText(String(this->battleInfo.enemy.curHP) + "/" + String(this->battleInfo.enemy.maxHP));
            case StatType::ATK:
                if(action.target == SkillTarget::SELF){
                    if(action.isHost == battleInfo.isHost){
                        this->pet->curATK += action.value;
                    }else{
                        this->battleInfo.enemy.curATK += action.value;
                    }
                }else{
                    if(action.isHost == battleInfo.isHost){
                        this->battleInfo.enemy.curATK -= action.value;
                    }else{
                        this->pet->curATK -= action.value;
                    }
                }
            case StatType::DEF:
                if(action.target == SkillTarget::SELF){
                    if(action.isHost == battleInfo.isHost){
                        this->pet->curDEF += action.value;
                    }else{
                        this->battleInfo.enemy.curDEF += action.value;
                    }
                }
            case StatType::SPD:
                if(action.target == SkillTarget::SELF){
                    if(action.isHost == battleInfo.isHost){
                        this->pet->curSPD += action.value;
                    }else{
                        this->battleInfo.enemy.curSPD += action.value;
                    }
                }
        }

        delay(300);
        this->battle->getChild("resolve")->setText(" ");
    }
    delay(300);
    this->battle->getChild("resolve")->setText("Selecione sua próxima ação");
    battleInfo.status = BattleStatus::PlayerTurn;
    battleInfo.actions.clear();
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
    if (millis() - lastPingTime > pingInterval) {
        PingModel send ={
            .message = WifiManager::getInstance()->GetDeviceID()
        };
        LoraManager::getInstance()->sendPacket<PingModel>(send, PING);
        lastPingTime = millis();
    }

    for (Packet packet : LoraManager::getInstance()->getPackets()) {
        Serial.println("Packet received: " + String(packet.type) + " from " + packet.source);
        if(packet.type == PING){
            UIElement* menu = this->battleSelect->getChild("menu");
            String challengeID = "challenge" + packet.source;
            UIElement* existing = menu->getChild(challengeID.c_str());
            if(existing != nullptr){continue;}
            PingModel msg = LoraManager::getInstance()->handlePacket<PingModel>(packet);
            int idx = menu->getChildAmount();
            UIElement* newPoint = new UIElement(challengeID.c_str(), 0, idx * 21, button, hoverButton, button);
            newPoint->setAction(BTN_A, [this, packet](UIElement* element) {
                LoraManager::getInstance()->connect(packet.source);

                BattleInviteModel invite = {
                    .sourcePet = {
                        .specie = this->pet->getSpecie(),
                        .name = this->pet->name,
                        .maxHP = this->pet->curHP,
                        .curHP = this->pet->curHP,
                        .curSPD = this->pet->curSPD,
                        .curDEF = this->pet->curDEF,
                        .curATK = this->pet->curATK
                    }
                };
                LoraManager::getInstance()->sendPacket<BattleInviteModel>(invite, BATTLE_INVITE);
            });
            newPoint->setText("Desafiar " + msg.message);
            menu->addChild(newPoint);
            menu->setSelectedIndex(idx);
        }
        if(packet.type == MESSAGE){
            String msg = LoraManager::getInstance()->handlePacket<String>(packet);
            if(msg == "BattleDecline"){
                LoraManager::getInstance()->disconnect();
            }
        }
        if(packet.type == BATTLE_INVITE){
            ShortPetData petShort = LoraManager::getInstance()->handlePacket<ShortPetData>(packet);
            LoraManager::getInstance()->connect(packet.source);
            this->battleSelect->getChild("acceptMessage")->setVisibility(true);
            this->battleSelect->getChild("menu")->setState(UIState::BASE);
            this->battleSelect->setSelectedIndex(1);
            this->battleSelect->getChild("acceptMessage")->setSelectedIndex(0);
            this->battleSelect->getChild("acceptMessage")->getChild("acceptMenu")->setState(UIState::SELECTED);
            this->battleInfo.enemy = petShort;
            this->battleInfo.isHost = false;
           
        }
        if(packet.type == BATTLE_START){
            BattleStartModel model = LoraManager::getInstance()->handlePacket<BattleStartModel>(packet);
            startBattle(true, model.seed, model.opponentPet);
        }
    }
    
}
void Mochilume::battleLoop(){
    for (Packet packet : LoraManager::getInstance()->getPackets()) {
        if(packet.type == BATTLE_SKILL && battleInfo.isHost && battleInfo.status == BattleStatus::WaitingEnemy){
            int enemySkill = LoraManager::getInstance()->handlePacket<int>(packet);
            battleInfo.enemySkill = enemySkill;
            if(battleInfo.selectedSkill != -1){
                this->resolveBattleTurn();
                
            }
        }

        if(packet.type == BATTLE_TURN && !battleInfo.isHost){
            BattleTurnModel model = LoraManager::getInstance()->handlePacket<BattleTurnModel>(packet);
            bool ok = true;
            if(ok){
                LoraManager::getInstance()->sendPacket<String>("OK", MESSAGE);
                passBattleActions(model.actions);
            }
        }

        if(packet.type == MESSAGE && battleInfo.isHost){
            String msg = LoraManager::getInstance()->handlePacket<String>(packet);
            if(msg == "OK"){
                passBattleActions(battleInfo.actions);
            }
        }
    }
}

void Mochilume::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}