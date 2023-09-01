#pragma once

#include <string>
#include "renderingHandler.h"

class CommandHandler {
public:
    static void processCommandInput(int cursorXPos, int cursorYPos);

private:
    static std::string collectCommand(RenderingHandler* renderer);
    static void executeCommand(std::string command, RenderingHandler* renderer);
};
