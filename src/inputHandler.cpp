#include <string>
#include <vector>
#include <ncurses.h>
#include "inputHandler.h"
#include "renderingHandler.h"
#include "selectionHandler.h"
#include "commandHandler.h"

bool InputHandler::selecting = FALSE;
const int InputHandler::KEY_SUP = KEY_MAX + 1;
const int InputHandler::KEY_SDOWN = KEY_MAX + 2;
const int InputHandler::KEY_RETURN = 10;;
const int InputHandler::KEY_ESC = 27;
const int InputHandler::KEY_TAB = 9;

void InputHandler::initializeKeyDefinitions() {
    define_key("\E[1;2A", KEY_SUP);
    define_key("\E[1;2B", KEY_SDOWN);
}

void InputHandler::processKeyInput(int key, int& cursorXPos, int& cursorYPos) {
    if (key == KEY_RESIZE) {}
    else if (key == KEY_ESC) {
        CommandHandler::processCommandInput(cursorXPos, cursorYPos);
    }
    else if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
        processArrowKey(key, cursorXPos, cursorYPos);
        if (selecting) {
            selecting = FALSE;
            SelectionHandler::initializeSelectedIndices();
        }
    }
    else if (key == KEY_RETURN) {
        processReturn(cursorXPos, cursorYPos);
        if (selecting) {
            selecting = FALSE;
            SelectionHandler::initializeSelectedIndices();
        }
    }
    else if (key == KEY_BACKSPACE) {
        processDelete(cursorXPos, cursorYPos);
        if (selecting) {
            selecting = FALSE;
            SelectionHandler::initializeSelectedIndices();
        }
    }
    else if (key == KEY_TAB) {
        processTab(cursorXPos, cursorYPos);
        if (selecting) {
            selecting = FALSE;
            SelectionHandler::initializeSelectedIndices();
        }
    }
    else if (key == KEY_SUP || key == KEY_SDOWN || key == KEY_SLEFT || key == KEY_SRIGHT) {
        selecting = TRUE;
        processShiftedArrowKey(key, cursorXPos, cursorYPos);
    }
    else {
        InputHandler::processNormalKey(key, cursorXPos, cursorYPos);
        if (selecting) {
            selecting = FALSE;
            SelectionHandler::initializeSelectedIndices();
        }
    }
}

void InputHandler::processNormalKey(int key, int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    char character = static_cast<char>(key);
    if (!selecting) {
        buffer[cursorYPos+currentTopLine].insert(cursorXPos, 1, character);
        cursorXPos++;
    }
    else {
        std::vector<std::vector<int>> selectedIndices = SelectionHandler::getSelectedIndices();
        int firstLine = selectedIndices[0][0];
        int lastLine = selectedIndices[1][0];
        std::string substrOne = buffer[firstLine].substr(0,selectedIndices[0][1]);
        std::string substrTwo = buffer[lastLine].substr(selectedIndices[1][1], static_cast<int>(buffer[lastLine].size()));
        for (int i = firstLine; i <= lastLine; i++) {
            buffer.erase(buffer.begin() + firstLine);
        }
        buffer.insert(buffer.begin() + firstLine, substrOne+character+substrTwo);
        cursorYPos = firstLine;
        cursorXPos = selectedIndices[0][1] + 1;
    }
}

void InputHandler::processTab(int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int& currentTopLine = renderer->getCurrentTopLine();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (!selecting) {
        buffer[cursorYPos + currentTopLine].insert(cursorXPos, 4, ' ');
        cursorXPos += 4;
    }
    else {
        std::vector<std::vector<int>> selectedIndices = SelectionHandler::getSelectedIndices();
        int firstLine = selectedIndices[0][0];
        int lastLine = selectedIndices[1][0];
        std::string substrOne = buffer[firstLine].substr(0,selectedIndices[0][1]);
        std::string substrTwo = buffer[lastLine].substr(selectedIndices[1][1], static_cast<int>(buffer[lastLine].size()));
        for (int i = firstLine; i <= lastLine; i++) {
            buffer.erase(buffer.begin() + firstLine);
        }
        buffer.insert(buffer.begin() + firstLine, substrOne+"    "+substrTwo);
        cursorYPos = firstLine;
        cursorXPos = selectedIndices[0][1] + 4;
    }
}

void InputHandler::processReturn(int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
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
        std::vector<std::vector<int>> selectedIndices = SelectionHandler::getSelectedIndices();
        int firstLine = selectedIndices[0][0];
        int lastLine = selectedIndices[1][0];
        std::string substrOne = buffer[firstLine].substr(0,selectedIndices[0][1]);
        std::string substrTwo = buffer[lastLine].substr(selectedIndices[1][1], static_cast<int>(buffer[lastLine].size()));
        for (int i = firstLine; i <= lastLine; i++) {
            buffer.erase(buffer.begin() + firstLine);
        }
        buffer.insert(buffer.begin() + firstLine, substrOne);
        buffer.insert(buffer.begin() + firstLine+1, substrTwo);
        cursorYPos = firstLine + 1 - renderer->getCurrentTopLine();
        cursorXPos = 0;
    }
}

void InputHandler::processDelete(int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
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
        std::vector<std::vector<int>> selectedIndices = SelectionHandler::getSelectedIndices();
        int firstLine = selectedIndices[0][0];
        int lastLine = selectedIndices[1][0];
        std::string substrOne = buffer[firstLine].substr(0,selectedIndices[0][1]);
        std::string substrTwo = buffer[lastLine].substr(selectedIndices[1][1], static_cast<int>(buffer[lastLine].size()));
        for (int i = firstLine; i <= lastLine; i++) {
            buffer.erase(buffer.begin() + firstLine);
        }
        buffer.insert(buffer.begin() + firstLine, substrOne+substrTwo);
        cursorYPos = firstLine-renderer->getCurrentTopLine();
        cursorXPos = selectedIndices[0][1];
    }
}

void InputHandler::processArrowKey(int key, int& cursorXPos, int& cursorYPos) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
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

void InputHandler::processShiftedArrowKey(int key, int& cursorXPos, int& cursorYPos) {
    std::string direction;
    if (key == KEY_SUP) {direction = "up";}
    else if (key == KEY_SDOWN) {direction = "down";}
    else if (key == KEY_SRIGHT) {direction = "right";}
    else if (key == KEY_SLEFT) {direction = "left";}
    SelectionHandler::updateSelectedIndices(cursorXPos, cursorYPos, direction);
    processArrowKey(key, cursorXPos, cursorYPos);
}

int InputHandler::collectInput() {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int input = wgetch(renderer->getWindow());
    return input;
}
