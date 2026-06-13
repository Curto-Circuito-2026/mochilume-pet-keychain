#ifndef REQUESTS_H
#define REQUESTS_H

#include <Arduino.h>
#include "Models.h"

class Requests {
private:
    static bool WaitForResponse(String& outResponse, bool& finishedFlag);

public:
    static bool RegisterPlayer(const PlayerAuthDto& authData);
    static bool LoginPlayer(const PlayerAuthDto& authData);
    static bool UploadSave(const PlayerDataDto& saveData);
    static bool DownloadSave(const char* username, PlayerDataDto& outSaveData);
};

#endif