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


bool LoraManager::isConnected() {
    return connection.isConnected;
}
void LoraManager::disconnect() {
    connection.isConnected = false;
    connection.destinationID = "0";
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
        Serial.println("Lora interrupt triggered");
        loraInterruptTriggered = true;
    }, RISING);
}

void LoraManager::loop()
{
    if(loraInterruptTriggered) {
        bool error = false;
        loraInterruptTriggered = false;
        String str;
        radio->readData(str);
        JsonDocument doc;
        deserializeJson(doc, str);
        Packet packet;
        packet.model = doc["model"].as<String>();       
        packet.source = doc["source"].as<String>();  
        if(packet.source == "0" || packet.source == "null") {error = true;}
        if(packet.source == connection.sourceID) {error = true;}
        packet.destination = doc["destination"].as<String>();     
        packet.type = static_cast<PacketType>(doc["type"].as<int>());
        if(packet.destination == "0" || packet.destination == connection.sourceID) {connection.timer = millis();}
        if(connection.isConnected && packet.source != connection.destinationID) {error = true;}

        if(!error){packetQueue.push(packet);}
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


