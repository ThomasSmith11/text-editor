#include "clipboard.h"
#include "inputHandler.h"
#include "selectionHandler.h"
#include "renderingHandler.h"
#include <ncurses.h>
#include <cstring>

std::vector<std::string> Clipboard::clipboard;

void Clipboard::copy() {
    doCopy();
}

void Clipboard::cut(int& cursorXPos, int& cursorYPos) {
    doCopy();
    InputHandler::processKeyInput(KEY_BACKSPACE, cursorXPos, cursorYPos);
}

void Clipboard::paste(int& cursorXPos, int&cursorYPos) {
    if (clipboard.empty()) {
        return;
    }
    if (InputHandler::selecting) {
        InputHandler::processKeyInput(KEY_BACKSPACE, cursorXPos, cursorYPos);
    }
    RenderingHandler* renderer = RenderingHandler::getInstance();
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    int& topLine = renderer->getCurrentTopLine();
    std::string substrOne = buffer[cursorYPos + topLine].substr(0, cursorXPos);
    std::string substrTwo = buffer[cursorYPos + topLine].substr(cursorXPos, buffer[cursorYPos + topLine].length());
    if (clipboard.size() == 1) {
        buffer[cursorYPos + topLine] = substrOne+clipboard[0]+substrTwo;
        cursorXPos += clipboard[0].length();
    }
    else {
        std::string firstLine = substrOne+clipboard[0];
        std::string lastLine = clipboard[static_cast<int>(clipboard.size())-1]+substrTwo;
        buffer[cursorYPos + topLine] = firstLine;
        for (int i=1; i<clipboard.size()-1; i++) {
            buffer.insert(buffer.begin() + cursorYPos + topLine+1, clipboard[i]);
            cursorYPos++;
            if (cursorYPos > LINES-1) {
                topLine++;
                cursorYPos--;
            }
        }
        buffer.insert(buffer.begin() + cursorYPos + topLine+1, lastLine);
        cursorYPos++;
        if (cursorYPos > LINES-1) {
            topLine++;
            cursorYPos--;
        }
        cursorXPos = clipboard[static_cast<int>(clipboard.size())-1].length();
    }
}

void Clipboard::doCopy() {
    clipboard.clear();
    std::vector<std::vector<int>> selectedIndices = SelectionHandler::getSelectedIndices();
    RenderingHandler* renderer = RenderingHandler::getInstance();
    std::vector<std::string> buffer = renderer->getDocument()->getBuffer();
    if (!InputHandler::selecting) {
        return;
    }
    if (selectedIndices[0][0] == selectedIndices[1][0]) {
        clipboard.push_back(buffer[selectedIndices[0][0]].substr(selectedIndices[0][1], selectedIndices[1][1]));
    }
    else {
        int currentx = selectedIndices[0][1];
        for (int i = selectedIndices[0][0]; i < selectedIndices[1][0]; i++) {
            clipboard.push_back(buffer[i].substr(currentx, buffer[i].length()));
            currentx = 0;
        }
        clipboard.push_back(buffer[selectedIndices[1][0]].substr(0, selectedIndices[1][1]));
    }
}

