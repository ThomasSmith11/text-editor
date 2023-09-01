#include <string>
#include <ncurses.h>
#include "inputHandler.h"
#include "renderingHandler.h"
#include "selectionHandler.h"
#include "commandHandler.h"

bool InputHandler::selecting = FALSE;
RenderingHandler* renderer = RenderingHandler::getInstance();
int KEY_SUP = KEY_MAX + 1;
int KEY_SDOWN = KEY_MAX + 2;

void InputHandler::initializeKeyDefinitions() {
    int KEY_SUP = KEY_MAX + 1;
    int KEY_SDOWN = KEY_MAX + 2;
    define_key("\E[1;2A", KEY_SUP);
    define_key("\E[1;2B", KEY_SDOWN);
}

void InputHandler::processKeyInput(int key, int& cursorXPos, int& cursorYPos) {
    if (key == KEY_RESIZE) {}
    else if (key == 27) {
        CommandHandler::processCommandInput(cursorXPos, cursorYPos);
    }
    else if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
        selecting = FALSE;
        InputHandler::processArrowKey(key, cursorXPos, cursorYPos);
    }
    else if (key == 10) {
        selecting = FALSE;
        InputHandler::processReturn(cursorXPos, cursorYPos);
    }
    else if (key == KEY_BACKSPACE) {
        selecting = FALSE;
        InputHandler::processDelete(cursorXPos, cursorYPos);
    }
    else if (key == 9) {
        selecting = FALSE;
        InputHandler::processTab(cursorXPos, cursorYPos);
    }
    else if (key == KEY_SUP || key == KEY_SDOWN || key == KEY_SLEFT || key == KEY_SRIGHT) {
        selecting = TRUE;
        InputHandler::processArrowKey(key, cursorXPos, cursorYPos);
    }
    else {
        selecting = FALSE;
        InputHandler::processNormalKey(key, cursorXPos, cursorYPos);
    }
}

void InputHandler::processNormalKey(int key, int& cursorXPos, int& cursorYPos) {
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    char character = static_cast<char>(key);
    if (!selecting) {
        buffer[cursorYPos+currentTopLine].insert(cursorXPos, 1, character);
        cursorXPos++;
    }
    else {
        //not yet implemented
    }
}

void InputHandler::processTab(int& cursorXPos, int& cursorYPos) {
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (!selecting) {
        buffer[cursorYPos + currentTopLine].insert(cursorXPos, 4, ' ');
        cursorXPos += 4;
    }
    else {
        //not yet implemented
    }
}

void InputHandler::processReturn(int& cursorXPos, int& cursorYPos) {
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (!selecting) {
        bool cursorAtEndOfLine = (static_cast<int>(buffer[cursorYPos + currentTopLine].size()) == cursorXPos);
        if (cursorAtEndOfLine) {
            cursorYPos++;
            cursorXPos = 0;
            buffer.insert(buffer.begin() + cursorYPos + currentTopLine, " ");
            if (cursorYPos>LINES-1 && currentTopLine+LINES-1 != buffer.size()-1) {
                currentTopLine++;
                cursorYPos=LINES-1;
            }
        }
        else {
            std::string substringOne = buffer[cursorYPos+currentTopLine].substr(0,cursorXPos);
            std::string substringTwo = buffer[cursorYPos+currentTopLine].substr(cursorXPos, static_cast<int>(buffer[cursorYPos + currentTopLine].size()));
            buffer[cursorYPos + currentTopLine] = substringOne;
            cursorYPos++;
            cursorXPos=0;
            buffer.insert(buffer.begin() + cursorYPos + currentTopLine, substringTwo);
            if (cursorYPos>LINES-1) {
                currentTopLine++;
                cursorYPos=LINES-1;
            }
        }
    }
    else {
        //not yet implemented
    }
}

void InputHandler::processDelete(int& cursorXPos, int& cursorYPos) {
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (!selecting) {
        if (cursorXPos != 0) {
            buffer[cursorYPos + currentTopLine].erase(cursorXPos-1, 1);
            cursorXPos--;
        }
        else if (cursorXPos == 0 && cursorYPos != 0) {
            std::string line = buffer[cursorYPos + currentTopLine];
            buffer.erase(buffer.begin() + cursorYPos + currentTopLine);
            cursorYPos--;
            cursorXPos = static_cast<int>(buffer[cursorYPos + currentTopLine].size());
            buffer[cursorYPos + currentTopLine] += line;
        }
        else if (cursorXPos == 0 && cursorYPos == 0 && currentTopLine != 0) {
            std::string line = buffer[cursorYPos + currentTopLine];
            buffer.erase(buffer.begin() + cursorYPos + currentTopLine);
            currentTopLine--;
            cursorXPos = static_cast<int>(buffer[cursorYPos + currentTopLine].size());
            buffer[cursorYPos + currentTopLine] += line;
        }
    }
    else {
        //not yet implemented
    }
}

void InputHandler::processArrowKey(int key, int& cursorXPos, int& cursorYPos) {
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (key == KEY_UP || key == KEY_SUP) {
        cursorYPos--;
        if (cursorYPos<0 && currentTopLine != 0) {
            currentTopLine--;
        }
    } else if (key == KEY_DOWN || key == KEY_SDOWN) {
        cursorYPos++;
        if (cursorYPos>LINES-1 && currentTopLine+LINES-1 < buffer.size()-1) {
            currentTopLine++;
        }
    } else if (key == KEY_RIGHT || key == KEY_SRIGHT) {
        cursorXPos++;
        if (cursorXPos > buffer[cursorYPos+currentTopLine].length() && cursorYPos+currentTopLine != buffer.size()-1) {
            cursorYPos++;
            cursorXPos=0;
            if (cursorYPos>LINES-1 && currentTopLine+LINES-1 < buffer.size()-1) {
                currentTopLine++;
            }
        }
    } else if (key == KEY_LEFT || key == KEY_SLEFT) {
        cursorXPos--;
        if (cursorXPos<0 && (currentTopLine != 0 || cursorYPos != 0)) {
            cursorYPos--;
            cursorXPos=buffer[cursorYPos+currentTopLine].length();
            if (cursorYPos<0 && currentTopLine != 0) {
                currentTopLine--;
            }
        }
    }
    cursorYPos = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(cursorYPos, LINES-1)));
    cursorXPos = std::max(0, std::min(cursorXPos, static_cast<int>(buffer[cursorYPos+currentTopLine].size())));
}

int InputHandler::collectInput() {
    int input = wgetch(renderer->getWindow());
    return input;
}
