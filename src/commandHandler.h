#pragma once

#include <string>
#include "renderingHandler.h"

class CommandHandler {
public:
    static void processCommandInput(int& cursorXPos, int& cursorYPos);

private:
    static std::string collectCommand(const char* directions = "");
    static void executeCommand(std::string command, int& cursorXPos, int& cursorYPos);
    static void orchestrateFind(RenderingHandler* renderer, int& cursorXPos, int& cursorYPos);
    static void doSearch(RenderingHandler* renderer, std::string searchTerm, int& cursorXPos, int& cursorYPos);
};
