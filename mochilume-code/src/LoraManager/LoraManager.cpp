#include "LoraManager.h"
#include <WifiManager.h>
#include <ArduinoJson.h>

LoraManager* LoraManager::_instance = nullptr;
static volatile bool loraInterruptTriggered = false;

bool LoraManager::connect(String destinationID) {
    connection.destinationID = destinationID;
    connection.isConnected = true;
    connection.timer = millis();
    return true;
}



std::vector<Packet> LoraManager::getPackets() {
    std::vector<Packet> packets;
    for(int i = 0; i < packetQueue.size(); i++) {
        Packet p = packetQueue.front();
        packetQueue.pop();
        packets.push_back(p);
    }
    return packets;
}


LoraManager::LoraManager() : hspi(HSPI) {
    connection.isConnected = false;
    connection.timer = 0;
    connection.sourceID = WifiManager::getInstance()->GetDeviceID();
    connection.destinationID = "0";
    
    hspi.begin(LORA_SPI_SCK, LORA_SPI_MISO, LORA_SPI_MOSI, LORA_CS);
    
    radio = new LLCC68(new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY, hspi));
    radio->begin(915.0, 125.0, 7, 5, 0x12, -9, 8, 0.0);
    radio->startReceive();
    
    attachInterrupt(digitalPinToInterrupt(LORA_DIO1), []() {
        loraInterruptTriggered = true;
    }, RISING);
}

void LoraManager::loop()
{
    if(loraInterruptTriggered) {
        loraInterruptTriggered = false;
        if(radio->available()) {
            String str;
            radio->read(str);
            JsonDocument doc;
            deserializeJson(doc, str);
            Packet packet;
            packet.model = doc["model"].as<String>();       
            packet.source = doc["source"].as<String>();     
            packet.destination = doc["destination"].as<String>();     
            packet.type = static_cast<PacketType>(doc["type"].as<int>());   
            if(packet.destination == "0" || packet.destination == connection.sourceID) {connection.timer = millis();}
            if(connection.isConnected && packet.source != connection.destinationID) {return;}
            packetQueue.push(packet);
        }
        radio->startReceive();
    }
    if(connection.isConnected && (millis() - connection.timer > LORA_CONNECTION_TIMEOUT)) {
        connection.isConnected = false;
        connection.destinationID = "0";
    }
}
LoraManager* LoraManager::getInstance() {
    if (_instance == nullptr) _instance = new LoraManager();
    return _instance;
}

template<typename T>
T LoraManager::handlePacket(Packet packet) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, packet.model);
    if(error) {
        Serial.print("Failed to parse packet JSON: ");
        Serial.println(error.f_str());
        return T(); 
    }
    switch(packet.type) {
        case PING: {
            PingModel model;
            model.message = doc["message"].as<String>();
            return model;
        }
        case PONG: {
            PongModel model;
            model.message = doc["message"].as<String>();
            return model;
        }
        case BATTLE_START: {
            BattleStartModel model;
            model.seed = doc["seed"].as<String>();
            model.opponentPet.specie = doc["opponentPet"]["specie"].as<int>();
            model.opponentPet.name = doc["opponentPet"]["name"].as<String>();
            model.opponentPet.maxHP = doc["opponentPet"]["maxHP"].as<int>();
            model.opponentPet.curHP = doc["opponentPet"]["curHP"].as<int>();
            model.opponentPet.curSPD = doc["opponentPet"]["curSPD"].as<int>();
            model.opponentPet.curDEF = doc["opponentPet"]["curDEF"].as<int>();
            model.opponentPet.curATK = doc["opponentPet"]["curATK"].as<int>();
            return model;
        }
        case BATTLE_SKILL: {
            BattleSkillModel model;
            model.skillID = doc["skillID"].as<int>();
            return model;
        }
        case BATTLE_TURN: {
            BattleTurnModel model;
            int actionsAmount = doc["actionsAmount"].as<int>();
            for(int i = 0; i < actionsAmount; i++) {
                BattleAction action;
                action.action = doc["actionsArray"][i]["action"].as<String>();
                action.result = doc["actionsArray"][i]["result"].as<String>();
                action.value = doc["actionsArray"][i]["value"].as<int>();
                action.stat = static_cast<StatType>(doc["actionsArray"][i]["stat"].as<int>());
                action.target = static_cast<SkillTarget>(doc["actionsArray"][i]["target"].as<int>());
                model.actions.push_back(action);
            }
            return model;
        }
        default:
            return T(); 
    }
}

