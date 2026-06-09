#include "Activities/Config.h"
#include "ActivityManager.h"
#include <DisplayManager.h>
#include "Assets.h"
#include <UI/UIMenu.h>
#include <UI/UIKeyboard.h>
#include "WifiManager.h"
#include "Requests.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

Config::Config() 
    : Activity("config", &config_logo) {}
void Config::setup() {
    this->createHomeScreen();
    this->createWifiDataScreen();
    this->createWifiListScreen();
    this->createDataAccountScreen();
    this->createRegisterAccountScreen();
    this->createLoginAccountScreen();

    _screen->changeScreen(home);
};


void Config::createHomeScreen(){
    home = new UIScreen();

    UIElement* WIFIConfig = new UIElement(
        "wifi", 
        0, 
        0, 
        button, 
        hoverButton, 
        button);
    WIFIConfig->setText("WIFI");

    UIElement* accountConfig = new UIElement(
        "account", 
        0, 
        30, 
        button, 
        hoverButton, 
        button);
    
    accountConfig->setText("Conta");

    UIElement* exitButton = new UIElement(
        "exitButton", 
        0, 
        60, 
        button, 
        hoverButton, 
        button);
    exitButton->setText("Sair");

    exitButton->setAction(BTN_A, [this](UIElement* element) { Serial.println("SAIR - MENU"); ActivityManager::getInstance()->setActivity("menu"); });
    WIFIConfig->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR WIFI"); _screen->changeScreen(wifiData); });
    accountConfig->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR CONTA"); _screen->changeScreen(dataAccount); });

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

    homeMenu->addChild(WIFIConfig);
    homeMenu->addChild(accountConfig);
    homeMenu->addChild(exitButton);
    homeMenu->setSelectedIndex(0);

    home->addChild(homeMenu);
    home->setSelectedIndex(0);
    homeMenu->setState(UIState::SELECTED);

    this->screens["home"] = home;
}

void Config::createWifiDataScreen(){
    this->wifiData = new UIScreen();

    UIElement* cWifiLabel = new UIElement("WIFILabel", 120, 30, text, text, text);
    cWifiLabel->setText("REDE ATUAL:");
    UIElement* cWifiName = new UIElement("WIFIName",110, 45, text, text, text);
    cWifiName->setText(WifiManager::getInstance()->currentNetwork.length() > 0? WifiManager::getInstance()->currentNetwork : "SEM REDE");
    
    UIMenu* wifiMenu = new UIMenu(
        "wifiMenu",
        80,
        80,
        emptyStyle,
        emptyStyle,
        emptyStyle,
        1,
        0,
        0,
        false
    );

    UIElement* searchWifi = new UIElement("search", 0, 0, button, hoverButton, button);
    searchWifi->setText("Buscar Redes");
    searchWifi->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR WIFI LIST"); _screen->changeScreen(wifiList); });

    UIElement* exit = new UIElement("exit", 0, 30, button, hoverButton, button);
    exit->setText("Voltar"); 
    exit->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });

    wifiMenu->addChild(searchWifi);
    wifiMenu->addChild(exit);
    wifiMenu->setSelectedIndex(0);

    wifiData->addChild(wifiMenu);
    wifiData->addChild(cWifiLabel);
    wifiData->addChild(cWifiName);
    wifiData->setSelectedIndex(0);
    wifiMenu->setState(UIState::SELECTED);
    this->screens["wifiData"] = wifiData;
}
void Config::createWifiListScreen(){
    this->wifiList = new UIScreen();

    UIMenu* listMenu = new UIMenu("listMenu", 40, 30, emptyStyle, emptyStyle, emptyStyle, 2, 0, 0, false);

    std::vector<String> networks = WifiManager::getInstance()->GetAvaliableWifis();

    UIKeyboard* keypad = new UIKeyboard("keypad", 0, 0);
    
    int c = 0;
    for(String s : networks){
        int y = floor(c/2);
        UIElement* network = new UIElement(s.c_str(), c%2 == 0 ? 0 : 80, y*20, button, hoverButton, button);
        c++;
            network->setText(s);
            network->setAction(BTN_A, [this, s, listMenu, keypad](UIElement* element) { 
                listMenu->setState(UIState::BASE); 
                keypad->toggle(true);
                this->wifiList->setSelectedIndex(keypad->index); 
                keypad->getChild("keyboard")->setState(UIState::SELECTED);

                keypad->onClose([listMenu, s, this](String typedText) {
                    if(typedText.length() > 0) {
                        bool connected = WifiManager::getInstance()->Connect(s, typedText);
                        if(connected){
                            this->wifiData->getChild("WIFIName")->setText(s);
                            this->_screen->changeScreen(this->wifiData);
                            WifiManager::getInstance()->Disconnect();
                        }
                    }
                    listMenu->getScreen()->setSelectedIndex(listMenu->index);        
                    listMenu->setState(UIState::SELECTED);
                });
        });
        listMenu->addChild(network);

    }

    UIElement* exit = new UIElement("exit", 30, 180, button, hoverButton, button);
    exit->setText("Voltar"); 
    exit->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR WIFIDATA"); _screen->changeScreen(wifiData); });

    listMenu->addChild(exit);
    listMenu->setSelectedIndex(0);
    listMenu->setState(UIState::SELECTED);

    wifiList->addChild(listMenu);
    wifiList->addChild(keypad);
    wifiList->setSelectedIndex(0);
    listMenu->setState(UIState::SELECTED);
    this->screens["wifiList"] = wifiList;
}

void Config::createDataAccountScreen(){
   this->dataAccount = new UIScreen();

    UIMenu* accountMenu = new UIMenu("accountMenu", 80, 50, emptyStyle, emptyStyle, emptyStyle, 1, 0, 0, false);

    String userName = "";
    bool loggedIn = false;
    Serial.println(F("[PlayerFS] Carregando dados do jogador..."));
    
    if (LittleFS.exists("/user.json")) {
        File file = LittleFS.open("/user.json", "r");
        
        if (file) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, file);
            if (!error) {
                const char* savedName = doc["userName"];
                Serial.println(doc.as<String>());
                bool isLogged = doc["isLoggedIn"] | false; 
                if (savedName != nullptr && strlen(savedName) > 0) {
                    userName = String(savedName);
                } 
                loggedIn = isLogged;
                Serial.println(F("[PlayerFS] Dados carregados com sucesso."));
                Serial.print(F("User: ")); Serial.println(userName);
                Serial.print(F("Logged: ")); Serial.println(loggedIn ? "SIM" : "NAO");
            }
            else 
            {
                Serial.println(F("[PlayerFS] Erro ao desserializar JSON: "));
                Serial.println(error.c_str());
            }
            
            file.close();
            }
    } 


    if(loggedIn){
        UIElement* saveCloud = new UIElement("saveCloud", 0, 0, button, hoverButton, button);
        saveCloud->setText("Salvar na Nuvem");
        saveCloud->setAction(BTN_A, [this](UIElement* element) {
            //arthur cria upload save aqui
        });

        UIElement* logout = new UIElement("logout", 0, 30, button, hoverButton, button);
        logout->setText("Deslogar");
        logout->setAction(BTN_A, [this](UIElement* element) {
            

            JsonDocument doc;

            if (LittleFS.exists("/user.json")) {
                File readFile = LittleFS.open("/user.json", "r");
                if (readFile) {
                    DeserializationError error = deserializeJson(doc, readFile);
                    readFile.close();
                }
            }

            doc["userName"] = "";
            doc["password"] = "";
            doc["isLoggedIn"] = false;

            File writeFile = LittleFS.open("/user.json", "w");

            if (serializeJson(doc, writeFile) == 0) {
                Serial.println(F("[Save] Falha ao gravar os dados no arquivo!"));
                writeFile.close();
            }
            writeFile.close();
            Serial.println(F("[Save] Dados de usuario salvos com sucesso no LittleFS!"));
            createDataAccountScreen();
            _screen->changeScreen(dataAccount);
        });

        accountMenu->addChild(logout);
        accountMenu->addChild(saveCloud);
  
    }else{
        UIElement* loginBtn = new UIElement("loginBtn", 0, 0, button, hoverButton, button);
        loginBtn->setText("Login");
        loginBtn->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR LOGIN"); _screen->changeScreen(loginAccount); });

        UIElement* registerBtn = new UIElement("registerBtn", 0, 30, button, hoverButton, button);
        registerBtn->setText("Registrar");
        registerBtn->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR REGISTER"); _screen->changeScreen(registerAccount); });
            
        accountMenu->addChild(loginBtn);
        //accountMenu->addChild(registerBtn);
    }
   
    UIElement* exit = new UIElement("exit", 0, 60, button, hoverButton, button);
    exit->setText("Voltar"); 
    exit->setAction(BTN_A, [this](UIElement* element) { Serial.println("IR HOME"); _screen->changeScreen(home); });

    accountMenu->addChild(exit);
    accountMenu->setSelectedIndex(0);
    accountMenu->setState(UIState::SELECTED);

    dataAccount->addChild(accountMenu);
    dataAccount->setSelectedIndex(0);
    accountMenu->setState(UIState::SELECTED);
    this->screens["dataAccount"] = dataAccount;
}
void Config::createLoginAccountScreen(){
    this->loginAccount = new UIScreen();

    UIMenu* loginFormMenu = new UIMenu("loginFormMenu", 80, 50, emptyStyle, emptyStyle, emptyStyle, 1, 4, 0, false);
    
    UIElement* userLabel = new UIElement("userLabel", 100, 40, text, text,text);
    userLabel->setText("User:");
    UIElement* userField = new UIElement("userField", 0, 0, field, hoverField, field);

    UIElement* passwordLabel = new UIElement("passLabel", 100, 80, text, text,text);
    passwordLabel->setText("Senha:");
    UIElement* passField = new UIElement("passField", 0, 40, field, hoverField, field);

    UIElement* loginConfirm = new UIElement("confirm", 0, 65, button, hoverButton, button);
    loginConfirm->setText("Entrar");

    UIElement* exit = new UIElement("exit", 0, 90, button, hoverButton, button);
    exit->setText("Voltar");

    loginFormMenu->addChild(userField);
    loginFormMenu->addChild(passField);
    loginFormMenu->addChild(loginConfirm);
    loginFormMenu->addChild(exit);

    UIKeyboard* keypad = new UIKeyboard("userKeyb", 0, 0);

    userField->setAction(BTN_A, [this,loginFormMenu, keypad](UIElement* element) {
        loginFormMenu->setState(UIState::BASE); 
        keypad->toggle(true);
        keypad->getChild("visualizer")->setText(element->getText());
        this->loginAccount->setSelectedIndex(keypad->index); 
        keypad->getChild("keyboard")->setState(UIState::SELECTED);

        keypad->onClose([loginFormMenu, element](String typedText) {
            if(typedText.length() > 0) element->setText(typedText);
            loginFormMenu->getScreen()->setSelectedIndex(loginFormMenu->index);        
            loginFormMenu->setState(UIState::SELECTED);
        });
    });

    passField->setAction(BTN_A, [this,loginFormMenu, keypad](UIElement* element) {
        loginFormMenu->setState(UIState::BASE); 
        keypad->toggle(true);
        keypad->getChild("visualizer")->setText(element->getText());
        this->loginAccount->setSelectedIndex(keypad->index); 
        keypad->getChild("keyboard")->setState(UIState::SELECTED);

        keypad->onClose([loginFormMenu, element](String typedText) {
            if(typedText.length() > 0) element->setText(typedText);
            loginFormMenu->getScreen()->setSelectedIndex(loginFormMenu->index);        
            loginFormMenu->setState(UIState::SELECTED);
        });
    });

    loginConfirm->setAction(BTN_A, [this, userField, passField](UIElement* element) {
        String username = userField->getText();
        String password = passField->getText();
        PlayerAuthDto authData;
        username.toCharArray(authData.userName, MAX_USERNAME_LEN);
        password.toCharArray(authData.password, MAX_PASSWORD_LEN);
        if (Requests::LoginPlayer(authData))
        {
            JsonDocument doc;

            if (LittleFS.exists("/user.json")) {
                File readFile = LittleFS.open("/user.json", "r");
                if (readFile) {
                    DeserializationError error = deserializeJson(doc, readFile);
                    readFile.close();
                }
            }

            doc["userName"] = username;
            doc["password"] = password;
            doc["isLoggedIn"] = true;

            File writeFile = LittleFS.open("/user.json", "w");
            if (!writeFile) {
                Serial.println(F("[Save] Falha ao abrir o arquivo para escrita!"));
            }

            if (serializeJson(doc, writeFile) == 0) {
                Serial.println(F("[Save] Falha ao gravar os dados no arquivo!"));
                writeFile.close();
            }

            writeFile.close();
            Serial.println(F("[Save] Dados de usuario salvos com sucesso no LittleFS!"));
        }


        this->createDataAccountScreen();
        this->_screen->changeScreen(dataAccount);
       
    });

    exit->setAction(BTN_A, [this](UIElement* element) { 
        Serial.println("VOLTAR CONTA"); 
        _screen->changeScreen(dataAccount); 
    });

    loginAccount->addChild(loginFormMenu);
    loginAccount->addChild(keypad);
    loginAccount->addChild(userLabel);
    loginAccount->addChild(passwordLabel);

    loginFormMenu->setSelectedIndex(0);
    loginAccount->setSelectedIndex(0);
    loginFormMenu->setState(UIState::SELECTED);
    
    this->screens["loginAccount"] = loginAccount;
}
void Config::createRegisterAccountScreen(){
    this->registerAccount = new UIScreen();
 this->loginAccount = new UIScreen();
    UIMenu* loginFormMenu = new UIMenu("loginFormMenu", 80, 50, emptyStyle, emptyStyle, emptyStyle, 1, 4, 0, false);

    UIElement* userLabel = new UIElement("userLabel", 100, 40, text, text,text);
    userLabel->setText("User:");
    UIElement* userField = new UIElement("userField", 0, 0, field, hoverField, field);

    UIElement* passwordLabel = new UIElement("passLabel", 100, 80, text, text,text);
    passwordLabel->setText("Senha:");
    UIElement* passField = new UIElement("passField", 0, 40, field, hoverField, field);

    UIElement* loginConfirm = new UIElement("confirm", 0, 65, button, hoverButton, button);
    loginConfirm->setText("Criar Conta");

    UIElement* exit = new UIElement("exit", 0, 90, button, hoverButton, button);
    exit->setText("Voltar");

    loginFormMenu->addChild(userField);
    loginFormMenu->addChild(passField);
    loginFormMenu->addChild(loginConfirm);
    loginFormMenu->addChild(exit);

    UIKeyboard* keypad = new UIKeyboard("userKeyb", 0, 0);

     userField->setAction(BTN_A, [this,loginFormMenu, keypad](UIElement* element) {
        loginFormMenu->setState(UIState::BASE); 
        keypad->toggle(true);
        keypad->getChild("visualizer")->setText(element->getText());
        this->loginAccount->setSelectedIndex(keypad->index); 
        keypad->getChild("keyboard")->setState(UIState::SELECTED);

        keypad->onClose([loginFormMenu, element](String typedText) {
            if(typedText.length() > 0) element->setText(typedText);
            loginFormMenu->getScreen()->setSelectedIndex(loginFormMenu->index);        
            loginFormMenu->setState(UIState::SELECTED);
        });
    });

    passField->setAction(BTN_A, [this,loginFormMenu, keypad](UIElement* element) {
        loginFormMenu->setState(UIState::BASE); 
        keypad->toggle(true);
        keypad->getChild("visualizer")->setText(element->getText());
        this->loginAccount->setSelectedIndex(keypad->index); 
        keypad->getChild("keyboard")->setState(UIState::SELECTED);

        keypad->onClose([loginFormMenu, element](String typedText) {
            if(typedText.length() > 0) element->setText(typedText);
            loginFormMenu->getScreen()->setSelectedIndex(loginFormMenu->index);        
            loginFormMenu->setState(UIState::SELECTED);
        });
    });

    loginConfirm->setAction(BTN_A, [this, userField, passField](UIElement* element) {
        String username = userField->getText();
        String password = passField->getText();
        PlayerAuthDto authData;
        username.toCharArray(authData.userName, MAX_USERNAME_LEN);
        password.toCharArray(authData.password, MAX_PASSWORD_LEN);
        Requests::LoginPlayer(authData);

        this->createDataAccountScreen();
        this->_screen->changeScreen(dataAccount);
       
    });

    exit->setAction(BTN_A, [this](UIElement* element) { 
        Serial.println("VOLTAR CONTA"); 
        _screen->changeScreen(dataAccount); 
    });

    registerAccount->addChild(loginFormMenu);
    registerAccount->addChild(keypad);
    registerAccount->addChild(userLabel);
    registerAccount->addChild(passwordLabel);

    loginFormMenu->setSelectedIndex(0);
    registerAccount->setSelectedIndex(0);
    loginFormMenu->setState(UIState::SELECTED);
    
    this->screens["registerAccount"] = registerAccount;
}


void Config::loop() {
    _screen->render();
    
}
void Config::stop() {
    for (std::pair<std::string, UIScreen *> c : this->screens){delete c.second;}
    this->screens.clear();
    
    Serial.println("stop");
}