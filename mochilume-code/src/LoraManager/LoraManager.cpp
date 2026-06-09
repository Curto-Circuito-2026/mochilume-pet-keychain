#include "LoraManager.h"
#include <WifiManager.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Utils.h>

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
    pendingPackets.clear();
}

std::vector<Packet> LoraManager::getPackets() {
    std::vector<Packet> packets;
    while (!packetQueue.empty()) {
        packets.push_back(packetQueue.front());
        packetQueue.pop();
    }
    return packets;
}


LoraManager::LoraManager() : hspi(HSPI) {
    connection.isConnected = false;
    connection.timer = 0;
    connection.sourceID = "";
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
        DeserializationError jsonErr = deserializeJson(doc, str);

        if(!jsonErr){
            if (doc.containsKey("ackId")) {
                String ackId = doc["ackId"].as<String>();
                for (auto it = pendingPackets.begin(); it != pendingPackets.end(); ) {
                    if (it->msgId == ackId) {
                        it = pendingPackets.erase(it);
                        Serial.println("System: Packet Delivery Confirmed for ID: " + ackId);
                    } else {
                        ++it;
                    }
                }
                
                error = true; 
            }
            else if(doc.containsKey("msgId")){
                String msgId = doc["msgId"].as<String>();
                
                JsonDocument ackDoc;
                ackDoc["source"] = connection.sourceID;
                ackDoc["destination"] = doc["source"].as<String>();
                ackDoc["type"] = static_cast<int>(PING); 
                ackDoc["ackId"] = msgId;
                
                String ackOutput;
                serializeJson(ackDoc, ackOutput);
                if (radio != nullptr) {
                    delay(50); 
                    radio->transmit(ackOutput);
                }

                if (msgId == lastProcessedMsgId) {
                    Serial.println("System: Dropping duplicate packet.");
                    error = true; 
                } else {
                    lastProcessedMsgId = msgId;
                }
            }
        

            Packet packet;
            if(doc.containsKey("model")){
                packet.model = doc["model"].as<String>();       
            }
            packet.source = doc["source"].as<String>();  
            if(packet.source == "0" || packet.source == "null" || packet.source == connection.sourceID) {error = true;}
            packet.destination = doc["destination"].as<String>();     
            packet.type = static_cast<PacketType>(doc["type"].as<int>());
            if(packet.destination == "0" || packet.destination == connection.sourceID) {connection.timer = millis();}
            if(connection.isConnected && packet.source != connection.destinationID) {error = true;}

            if(!error){packetQueue.push(packet);}
        }
        radio->startReceive();
    }

    for (auto& packet : pendingPackets) {
        if (packet.active && (millis() - packet.lastSent > 1200)) {
            Serial.println("System: Resending unacknowledged packet ID: " + packet.msgId);
            if(radio != nullptr) {
                radio->transmit(packet.payload);
                radio->startReceive();
            }
            packet.lastSent = millis();
        }
        
    }
    
    if(connection.isConnected && (millis() - connection.timer > LORA_CONNECTION_TIMEOUT)) {
        this->disconnect();
    }
}

String LoraManager::getDeviceID(){
    if(connection.sourceID.length() > 0){
        return connection.sourceID;
    }

    String id = "";

    if (LittleFS.exists("/user.json")) {
        File file = LittleFS.open("/user.json", "r");
        
        if (file) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, file);
            
            if (!error) {
                if (doc.containsKey("userID") && doc["userID"].is<const char*>()) {
                    const char* idChar = doc["userID"];
                    id = String(idChar);
                    id.trim();
                    if(id.length() > 0){
                        file.close();
                        return id;
                    }
                }
            }
            file.close();
        }
    }

    id = generateRandomString(8);
    Serial.println(id);

    return id;
}
LoraManager* LoraManager::getInstance() {
    if (_instance == nullptr) _instance = new LoraManager();
    return _instance;
}

void LoraManager::begin() {
    connection.sourceID = LoraManager::getDeviceID();
}

