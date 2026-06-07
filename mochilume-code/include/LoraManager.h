

#ifndef LORA_MANAGER_H    
#define LORA_MANAGER_H

#include <Arduino.h>
#include "Activities/Mochilume.h"
#include "HalConfig.h"
#include "RadioLib.h"
#include <ArduinoJson.h>
#include "PacketModels.h"
#include "PetData.h"

#define LORA_CONNECTION_TIMEOUT 60000 // Tempo limite para considerar uma conexão perdida (30 segundos)

struct LoraConnection {
    long timer;
    String destinationID;
    String sourceID;
    bool isConnected;
};


class LoraManager {
    private:
        static LoraManager* _instance;
        LoraManager();
        LLCC68* radio = nullptr;
        SPIClass hspi;
        LoraConnection connection;
        std::queue<Packet> packetQueue;
    public:
        static LoraManager* getInstance();

        bool connect(String destinationID);

        std::vector<Packet> getPackets();

        bool isConnected();
        void disconnect();

        template<typename T>
        void sendPacket(T message, PacketType type) {
            Packet packet;
            packet.source = connection.sourceID;
            packet.destination = connection.destinationID; 
            if(packet.destination != "0" && !connection.isConnected) {return;}
            packet.type = type;

            // Criamos um ponteiro genérico para a mensagem original
            const void* msgPtr = &message;

            JsonDocument doc;
            switch(type) {
                case PING: {
                    // CORREÇÃO: Cast estático/ponteiro força o compilador a aceitar o tipo correto apenas nesta ramificação
                    const PingModel& model = *static_cast<const PingModel*>(msgPtr);
                    doc["message"] = model.message;
                    break;
                }
                case PONG: {
                    const PongModel& model = *static_cast<const PongModel*>(msgPtr);
                    doc["message"] = model.message;
                    break;
                }
                case MESSAGE: {
                    const String& model = *static_cast<const String*>(msgPtr);
                    doc = model;
                    break;
                }
                case BATTLE_START: {
                    const BattleStartModel& model = *static_cast<const BattleStartModel*>(msgPtr);
                    doc["seed"] = model.seed;
                    doc["opponentPet"]["specie"] = model.opponentPet.specie;
                    doc["opponentPet"]["name"] = model.opponentPet.name;
                    doc["opponentPet"]["maxHP"] = model.opponentPet.maxHP;
                    doc["opponentPet"]["curHP"] = model.opponentPet.curHP;
                    doc["opponentPet"]["curSPD"] = model.opponentPet.curSPD;
                    doc["opponentPet"]["curDEF"] = model.opponentPet.curDEF;
                    doc["opponentPet"]["curATK"] = model.opponentPet.curATK;
                    doc["isHost"] = model.isHost;
                    break;
                }
                case BATTLE_INVITE: {
                    const BattleInviteModel& model = *static_cast<const BattleInviteModel*>(msgPtr);
                    doc["sourcePet"]["specie"] = model.sourcePet.specie;
                    doc["sourcePet"]["name"] = model.sourcePet.name;
                    doc["sourcePet"]["maxHP"] = model.sourcePet.maxHP;
                    doc["sourcePet"]["curHP"] = model.sourcePet.curHP;
                    doc["sourcePet"]["curSPD"] = model.sourcePet.curSPD;
                    doc["sourcePet"]["curDEF"] = model.sourcePet.curDEF;
                    doc["sourcePet"]["curATK"] = model.sourcePet.curATK;
                    break;
                }
                case BATTLE_SKILL: {
                    const BattleSkillModel& model = *static_cast<const BattleSkillModel*>(msgPtr);
                    doc["skillID"] = model.skillID;
                    break;
                }
                case BATTLE_TURN: {
                    const BattleTurnModel& model = *static_cast<const BattleTurnModel*>(msgPtr);
                    doc["actionsAmount"] = model.actions.size();
                    
                    JsonArray jsonActions = doc["actionsArray"].to<JsonArray>();
                    
                    for(size_t i = 0; i < model.actions.size(); i++) {
                        JsonObject actObj = jsonActions.add<JsonObject>();
                        actObj["action"] = model.actions[i].action;
                        actObj["result"] = model.actions[i].result;
                        actObj["value"]  = model.actions[i].value;
                        actObj["isHost"] = model.actions[i].isHost;
                        actObj["stat"]   = static_cast<int>(model.actions[i].stat);
                        actObj["target"] = static_cast<int>(model.actions[i].target);
                    }
                    break;
                }
            }
            
            String output;
            serializeJson(doc, output);
            packet.model = output;
            
            JsonDocument packetDoc;
            packetDoc["source"] = packet.source;
            packetDoc["destination"] = packet.destination;
            packetDoc["type"] = static_cast<int>(packet.type);
            packetDoc["model"] = packet.model;
            
            String packetOutput;
            serializeJson(packetDoc, packetOutput);
            
            if (radio != nullptr) {
                radio->transmit(packetOutput);
                radio->startReceive();
            }
        };
        void loop();

        template<typename T>
        T handlePacket(Packet packet) {
            T resultContainer; 
            
            void* resultPtr = &resultContainer;

            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, packet.model);
            if(error) {
                Serial.print("Failed to parse packet JSON: ");
                Serial.println(error.f_str());
                return resultContainer; 
            }
            
            switch(packet.type) {
                case PING: {
                    PingModel* model = static_cast<PingModel*>(resultPtr);
                    model->message = doc["message"].as<String>();
                    break;
                }
                case PONG: {
                    PongModel* model = static_cast<PongModel*>(resultPtr);
                    model->message = doc["message"].as<String>();
                    break;
                }
                case MESSAGE: {
                    String* model = static_cast<String*>(resultPtr);
                    *model = doc.as<String>();
                    break;
                }
                case BATTLE_START: {
                    BattleStartModel* model = static_cast<BattleStartModel*>(resultPtr);
                    model->seed = doc["seed"].as<String>();
                    model->isHost = doc["isHost"].as<bool>();
                    model->opponentPet.specie = doc["opponentPet"]["specie"].as<int>();
                    model->opponentPet.name   = doc["opponentPet"]["name"].as<String>();
                    model->opponentPet.maxHP  = doc["opponentPet"]["maxHP"].as<int>();
                    model->opponentPet.curHP  = doc["opponentPet"]["curHP"].as<int>();
                    model->opponentPet.curSPD = doc["opponentPet"]["curSPD"].as<int>();
                    model->opponentPet.curDEF = doc["opponentPet"]["curDEF"].as<int>();
                    model->opponentPet.curATK = doc["opponentPet"]["curATK"].as<int>();
                    break;
                }
                case BATTLE_INVITE: {
                    BattleInviteModel* model = static_cast<BattleInviteModel*>(resultPtr);
                    model->sourcePet.specie = doc["sourcePet"]["specie"].as<int>();
                    model->sourcePet.name   = doc["sourcePet"]["name"].as<String>();
                    model->sourcePet.maxHP  = doc["sourcePet"]["maxHP"].as<int>();
                    model->sourcePet.curHP  = doc["sourcePet"]["curHP"].as<int>();
                    model->sourcePet.curSPD = doc["sourcePet"]["curSPD"].as<int>();
                    model->sourcePet.curDEF = doc["sourcePet"]["curDEF"].as<int>();
                    model->sourcePet.curATK = doc["sourcePet"]["curATK"].as<int>();
                    break;
                }
                case BATTLE_SKILL: {
                    BattleSkillModel* model = static_cast<BattleSkillModel*>(resultPtr);
                    model->skillID = doc["skillID"].as<int>();
                    break;
                }
                case BATTLE_TURN: {
                    BattleTurnModel* model = static_cast<BattleTurnModel*>(resultPtr);
                    int actionsAmount = doc["actionsAmount"].as<int>();
                    for(int i = 0; i < actionsAmount; i++) {
                        BattleAction action;
                        action.action = doc["actionsArray"][i]["action"].as<String>();
                        action.result = doc["actionsArray"][i]["result"].as<String>();
                        action.value  = doc["actionsArray"][i]["value"].as<int>();
                        action.isHost = doc["actionsArray"][i]["isHost"].as<bool>();
                        action.stat   = static_cast<StatType>(doc["actionsArray"][i]["stat"].as<int>());
                        action.target = static_cast<SkillTarget>(doc["actionsArray"][i]["target"].as<int>());
                        model->actions.push_back(action);
                    }
                    break;
                }
                default:
                    break;
            }
            
            // 3. Retorna o container 'T' preenchido de forma disfarçada pelo ponteiro
            return resultContainer; 
        }

};

#endif