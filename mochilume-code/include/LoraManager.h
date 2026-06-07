#include <Arduino.h>
#include "Activities/Mochilume.h"
#include "HalConfig.h"
#include "RadioLib.h"
#include <ArduinoJson.h>


enum PacketType {
    PING,       // Descobrir quem está perto (Lobby)
    PONG,       // Responder ao sinal de Ping
    MESSAGE,    // Conversa direta privada P2P
    BATTLE_START,     // Informação de início de batalha
    BATTLE_SKILL,      // Informação de movimento em batalha 
    BATTLE_TURN         // Informação de término do round
};

struct Packet {
    String source;      // Quem está enviando (ID do seu ESP32)
    String destination;     // Para quem vai (Use 0 para mandar para TODO MUNDO / Lobby)
    PacketType type;      // PING, PONG ou MESSAGE
    String model;         // O conteúdo da mensagem de texto
};


struct PingModel {
    String message; 
};
struct PongModel {
    String message;
};

struct BattleStartModel {
    String seed; // Semente para geração de números aleatórios (opcional, mas útil para sincronização)
    ShortPetData opponentPet; // Informações básicas do pet adversário
}; 
struct BattleSkillModel {
    int skillID; // ID da skill usada
};

struct BattleAction{
    String action; // Descrição textual da ação (ex: "Pet A usou Skill X!");
    String result; // Descrição do resultado (ex: "Pet B perdeu 20 HP!");
    int value;
    StatType stat; // Qual stat foi afetada, se aplicável
    SkillTarget target; // Quem foi afetado (SELF ou OTHER)
};
struct BattleTurnModel {
    std::vector<BattleAction> actions; // Lista de ações ocorridas no turno;
};

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
        T handlePacket(Packet packet);
};