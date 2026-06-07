#include <Arduino.h>
#include "Activities/Mochilume.h"

enum PacketType {
    PING,       // Descobrir quem está perto (Lobby)
    PONG,       // Responder ao sinal de Ping
    MESSAGE,    // Conversa direta privada P2P,
    BATTLE_INVITE,
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

struct BattleInviteModel {
    ShortPetData sourcePet; // Informações básicas do pet adversário
};
struct BattleStartModel {
    String seed; // Semente para geração de números aleatórios (opcional, mas útil para sincronização)
    ShortPetData opponentPet; // Informações básicas do pet adversário
    bool isHost; // Indica se o remetente é o host da batalha
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