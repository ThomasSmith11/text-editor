#pragma once

#include <vector>
#include <string>
#include "renderingHandler.h"

class InputHandler {
public:
    static bool selecting;
    static void processKeyInput(int key, int& cursorXPos, int& cursorYPos);
    static int collectInput();
    static void initializeKeyDefinitions();
private:
    static void processNormalKey(int key, int& cursorXPos, int& cursorYPos);
    static void processTab(int& cursorXPos, int& cursorYPos);
    static void processReturn(int& cursorXPos, int& cursorYPos);
    static void processDelete(int& cursorXPos, int& cursorYPos);
    static void processArrowKey(int key, int& cursorXPos, int& cursorYPos);
};
