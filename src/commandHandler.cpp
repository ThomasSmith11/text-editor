#include "commandHandler.h"
#include "renderingHandler.h"
#include "inputHandler.h"
#include <vector>
#include <string>

void CommandHandler::processCommandInput(int cursorXPos, int cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    std::string command = collectCommand(renderer);
    executeCommand(command, renderer);
}

std::string CommandHandler::collectCommand(RenderingHandler* renderer) {
    std::string input;
    const char* cStr = "";
    renderer->renderCommand(cStr);
    int nextChar;
    nextChar = InputHandler::collectInput();
    while (nextChar != 10) {
        if (nextChar == KEY_BACKSPACE && !input.empty()) {
            input.pop_back();
        }
        else if (nextChar != KEY_BACKSPACE) {
            input += static_cast<char>(nextChar);
        }
        const char* cStr = input.c_str();
        renderer->renderCommand(cStr);
        nextChar = InputHandler::collectInput();
    }
    return input;
}

void CommandHandler::executeCommand(std::string command, RenderingHandler* renderer) {
    if (command == ":q"){
        renderer->closeAndDeleteWindow();
        exit(0);
    }
    if (command == ":c") {

    }
    if (command == ":x") {

    }
    if (command == ":v") {

    }
    if (command == ":qs" || command == ":sq"){
        renderer->getDocument()->saveDoc();
        renderer->closeAndDeleteWindow();
        exit(0);
    }
    if (command == ":s"){
        renderer->getDocument()->saveDoc();
    }
    if (command == ":f"){
        std::vector<std::string> buffer = renderer->getDocument()->getBuffer();
    }
    renderer->renderDoc();
}

