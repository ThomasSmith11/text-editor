#include "commandHandler.h"
#include "renderingHandler.h"
#include "inputHandler.h"
#include "selectionHandler.h"
#include "clipboard.h"
#include <vector>
#include <string>
#include <regex>

void CommandHandler::processCommandInput(int& cursorXPos, int& cursorYPos) {
    std::string command = collectCommand();
    executeCommand(command, cursorXPos, cursorYPos);
}

std::string CommandHandler::collectCommand(const char* directions) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    std::string input;
    const char* cStr = "";
    renderer->renderCommand(cStr, directions);
    int nextChar;
    nextChar = InputHandler::collectInput();
    while (nextChar != InputHandler::KEY_RETURN) {
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

void CommandHandler::executeCommand(std::string command, int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    if (command == ":q"){
        delete renderer;
        exit(0);
    }
    if (command == ":c") {
        Clipboard::copy();
    }
    if (command == ":x") {
        Clipboard::cut(cursorXPos, cursorYPos);
    }
    if (command == ":v") {
        Clipboard::paste(cursorXPos, cursorYPos);
    }
    if (command == ":qs" || command == ":sq"){
        renderer->getDocument()->saveDoc();
        delete renderer;
        exit(0);
    }
    if (command == ":s"){
        renderer->getDocument()->saveDoc();
    }
    if (command == ":f"){
        InputHandler::selecting=FALSE;
        SelectionHandler::initializeSelectedIndices();
        orchestrateFind(renderer, cursorXPos, cursorYPos);
    }
    renderer->renderDoc();
}

void CommandHandler::orchestrateFind(RenderingHandler* renderer, int& cursorXPos, int& cursorYPos) {
    const char* command = "";
    const char* description = "Type value to search for, then enter: ";
    renderer->renderCommand(command, description);
    std::string searchTerm = collectCommand(description);
    doSearch(renderer, searchTerm, cursorXPos, cursorYPos);
}

void CommandHandler::doSearch(RenderingHandler* renderer, std::string searchTerm, int& cursorXPos, int& cursorYPos) {
    std::vector<std::string> buffer = renderer->getDocument()->getBuffer();
    const std::regex expression(searchTerm, std::regex_constants::icase);

    int currentLine = 0;
    std::vector<std::vector<int> > matches;
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

    int nextChar = InputHandler::KEY_RETURN;
    while (TRUE) {
        for (std::vector<int> match : matches) {
            while (nextChar != InputHandler::KEY_RETURN && nextChar != InputHandler::KEY_ESC) {
                nextChar = InputHandler::collectInput();
            }
            if (nextChar == InputHandler::KEY_ESC) {
                break;
            }
            int line = match[0];
            int index = match[1];
            renderer->displayHighlightedSearchTerm(searchTerm, cursorXPos, cursorYPos, line, index);
            nextChar = 0;
        }
        if (nextChar == InputHandler::KEY_ESC) {
                break;
        }
    }
}
