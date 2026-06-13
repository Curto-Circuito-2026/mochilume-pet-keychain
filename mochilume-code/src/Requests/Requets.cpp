#include "Requests.h"
#include <ArduinoJson.h>
#include "WifiManager.h"
#include "HalConfig.h"

bool Requests::WaitForResponse(String& outResponse, bool& finishedFlag) {
    unsigned long startAttempt = millis();
    while (!finishedFlag) {
        delay(10); 
        if (millis() - startAttempt > WIFI_CONNECTION_TIMEOUT) {
            Serial.println("[Requests] Erro: Timeout na resposta do servidor.");
            return false;
        }
    }
    return true;
}

static bool ParseServiceResponse(const String& response) {
    if (response.length() == 0) return false;

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) return false;

    if (doc.containsKey("hasError")) {
        if (doc["hasError"].as<bool>()) {
            const char* message = doc["message"];
            if (message) Serial.println(message);
            return false;
        }
        return true;
    }

    return true;
}

bool Requests::RegisterPlayer(const PlayerAuthDto& authData) {
    WifiManager* wm = WifiManager::getInstance();
    wm->ReConnect();
    if (!wm->isConnected) {
        Serial.println("[Requests] Sem conexao Wi-Fi para registrar.");
        return false;
    }

    StaticJsonDocument<128> doc;
    doc["userName"] = authData.userName;
    doc["password"] = authData.password;
    
    String payload;
    serializeJson(doc, payload);

    String response = "";
    bool finished = false;

    Serial.println("[Requests] Enviando POST Register...");
    if (!wm->Fetch(AUTH_REGISTER_ENDPOINT, POST, payload, response, finished)) return false;
    if (!WaitForResponse(response, finished)) return false;

    Serial.print("[Requests] Resposta Register: ");
    Serial.println(response.c_str());
    wm->Disconnect();
    return ParseServiceResponse(response);
}

// 2. LOGIN
bool Requests::LoginPlayer(const PlayerAuthDto& authData) {
    WifiManager* wm = WifiManager::getInstance();
    wm->ReConnect();
    if (!wm->isConnected) return false;

    StaticJsonDocument<128> doc;
    doc["userName"] = authData.userName;
    doc["password"] = authData.password;
    
    String payload;
    serializeJson(doc, payload);

    String response = "";
    bool finished = false;

    Serial.println(authData.userName);
    Serial.println(authData.password);
    Serial.println("[Requests] Enviando POST Login...");
    if (!wm->Fetch(AUTH_LOGIN_ENDPOINT, POST, payload, response, finished)) return false;
    if (!WaitForResponse(response, finished)) return false;

    Serial.print("[Requests] Resposta Login: ");
    Serial.println(response.c_str());
    wm->Disconnect();
    return ParseServiceResponse(response);
}

// 3. SAVE UPLOAD
bool Requests::UploadSave(const PlayerDataDto& saveData) {
    Serial.println("[Requests] Preparando dados para upload...");
    WifiManager* wm = WifiManager::getInstance();
    wm->ReConnect();
    if (!wm->isConnected) return false;

    DynamicJsonDocument doc(2048); 
    doc["id"] = saveData.id;
    doc["userName"] = saveData.userName;
    doc["steps"] = saveData.steps;
    
    JsonArray petsArray = doc.createNestedArray("pets");
    for (int i = 0; i < saveData.petsCount; i++) {
        JsonObject petObj = petsArray.createNestedObject();
        petObj["id"] = saveData.pets[i].id;
        petObj["name"] = saveData.pets[i].name;
        petObj["level"] = saveData.pets[i].level;
        petObj["xp"] = saveData.pets[i].xp;
        petObj["species"] = saveData.pets[i].species;
        petObj["isActive"] = saveData.pets[i].isActive;
    }

    String payload;
    serializeJson(doc, payload);

    String response = "";
    bool finished = false;

    Serial.println("[Requests] Enviando PUT Save Upload...");
    Serial.println(payload);
    if (!wm->Fetch(SAVE_UPLOAD_ENDPOINT, PUT, payload, response, finished)) return false;
    if (!WaitForResponse(response, finished)) return false;
    Serial.print("[Requests] Resposta Save Upload: ");
    Serial.println(response.c_str());
    wm->Disconnect();
    return ParseServiceResponse(response);
}

// 4. SAVE DOWNLOAD
bool Requests::DownloadSave(const char* username, PlayerDataDto& outSaveData) {
    WifiManager* wm = WifiManager::getInstance();
    wm->ReConnect();
    if (!wm->isConnected) return false;

    char fullUrl[256];
    snprintf(fullUrl, sizeof(fullUrl), "%s%s%s", SAVE_DOWNLOAD_BASE_URL, username, SAVE_DOWNLOAD_SUFFIX);

    String response = "";
    bool finished = false;

    Serial.printf("[Requests] Enviando GET Save Download para %s...\n", username);
    if (!wm->Fetch(fullUrl, response, finished)) return false;
    if (!WaitForResponse(response, finished)) return false;

    if (response.length() == 0) return false;

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Serial.print("[Requests] Erro ao processar JSON de Download: ");
        Serial.println(error.c_str());
        return false;
    }

    outSaveData.id = doc["model"]["id"];
    Serial.println(response);
    strlcpy(outSaveData.userName, doc["model"]["userName"] | "", sizeof(outSaveData.userName));
    outSaveData.steps = doc["model"]["steps"];

    JsonArray petsArray = doc["model"]["pets"].as<JsonArray>();
    Serial.println(petsArray);
    outSaveData.petsCount = 0;

    for (JsonObject petObj : petsArray) {
        if (outSaveData.petsCount >= MAX_PETS_INVENTORY) break;

        int idx = outSaveData.petsCount;
        outSaveData.pets[idx].id = petObj["id"];
        strlcpy(outSaveData.pets[idx].name, petObj["name"] | "", sizeof(outSaveData.pets[idx].name));
        outSaveData.pets[idx].level = petObj["level"];
        outSaveData.pets[idx].xp = petObj["xp"];
        outSaveData.pets[idx].species = petObj["species"];
        outSaveData.pets[idx].isActive = petObj["isActive"];

        outSaveData.petsCount++;
    }

    Serial.println("[Requests] Save baixado e populado com sucesso!");
    wm->Disconnect();
    return true;
}