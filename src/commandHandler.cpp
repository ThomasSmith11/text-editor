#include "commandHandler.h"
#include "renderingHandler.h"
#include "inputHandler.h"
#include <vector>
#include <string>
#include <regex>

void CommandHandler::processCommandInput(int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    std::string command = collectCommand(renderer);
    executeCommand(command, renderer, cursorXPos, cursorYPos);
}

std::string CommandHandler::collectCommand(RenderingHandler* renderer, const char* directions) {
    std::string input;
    const char* cStr = "";
    renderer->renderCommand(cStr, directions);
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
        renderer->renderCommand(cStr, directions);
        nextChar = InputHandler::collectInput();
    }
    return input;
}

void CommandHandler::executeCommand(std::string command, RenderingHandler* renderer, int& cursorXPos, int& cursorYPos) {
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
        orchestrateFind(renderer, cursorXPos, cursorYPos);
    }
    renderer->renderDoc();
}

void CommandHandler::orchestrateFind(RenderingHandler* renderer, int& cursorXPos, int& cursorYPos) {
    const char* command = "";
    const char* description = "Type value to search for, then enter: ";
    renderer->renderCommand(command, description);
    std::string searchTerm = collectCommand(renderer, description);
    doSearch(renderer, searchTerm, cursorXPos, cursorYPos);
}

void CommandHandler::doSearch(RenderingHandler* renderer, std::string searchTerm, int& cursorXPos, int& cursorYPos) {
    std::vector<std::string> buffer = renderer->getDocument()->getBuffer();
    const std::regex expression(searchTerm, std::regex_constants::icase);

    int currentLine = 0;
    std::vector<std::vector<int>> matches;
    for (std::string line : buffer) {
        std::smatch m;
        while (std::regex_search(line, m, expression)) {
            int index = m.position();
            std::vector<int> match;
            match.push_back(currentLine);
            match.push_back(index);
            matches.push_back(match);
            line = m.suffix().str();
        }
        currentLine++;
    }

    int nextChar = 10;
    while (TRUE) {
        for (std::vector match : matches) {
            while (nextChar != 10 && nextChar != 27) {
                nextChar = InputHandler::collectInput();
            }
            if (nextChar == 27) {
                break;
            }
            int line = match[0];
            int index = match[1];
            renderer->displayHighlightedSearchTerm(searchTerm, cursorXPos, cursorYPos, line, index);
            nextChar = 0;
        }
        if (nextChar == 27) {
                break;
        }
    }
}
