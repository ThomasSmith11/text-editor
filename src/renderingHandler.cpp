#include "renderingHandler.h"
#include "selectionHandler.h"
#include <ncurses.h>
#include <vector>
#include <cstring>

RenderingHandler* RenderingHandler::instance = nullptr;

RenderingHandler::RenderingHandler() : currentTopLine(0), document(nullptr), window(nullptr) {}

RenderingHandler* RenderingHandler::getInstance() {
    if (instance == nullptr) {
        initscr();
        cbreak();
        noecho();
        ESCDELAY = 0;
        WINDOW *textwin = newwin(LINES, COLS, 0, 0);
        keypad(textwin, TRUE);
        instance = new RenderingHandler();
        instance->setWindow(textwin);
    }
    return instance;
}

RenderingHandler::~RenderingHandler() {
    delwin(this->window);
    endwin();
}

void RenderingHandler::setDocument(Document* doc) {
    this->document = doc;
    initiallyResize();
}

Document* RenderingHandler::getDocument() {
    return this->document;
}

void RenderingHandler::setWindow(WINDOW* window) {
    this->window = window;
}

WINDOW* RenderingHandler::getWindow() {
    return this->window;
}

int& RenderingHandler::getCurrentTopLine() {
    return this->currentTopLine;
}

void RenderingHandler::moveCursor(int cursorXpos, int cursorYpos){
    wmove(this->window, cursorYpos, cursorXpos);
}

void RenderingHandler::renderDoc(int linesToRemove) {
    wclear(this->window);
    int linesToPrint = LINES-linesToRemove;
    int i = 0;
    std::vector<std::string> buffer = this->document->getBuffer();
    while (i < linesToPrint) {
        if (this->currentTopLine + i >= static_cast<int>(buffer.size())) {
            break;
        }
        std::string line = buffer[this->currentTopLine + i];
        if (line.back() != '\n') {
            line += "\n";
        }
        const char* cStr = line.c_str();
        wchgat(this->window, line.length(), A_NORMAL, 0, NULL);
        wprintw(this->window, "%s", cStr);
        i++;
    }
    wrefresh(this->window);
}

void RenderingHandler::renderSelected(int linesToRemove) {
    std::vector<std::vector<int>> selectedIndices = SelectionHandler::getSelectedIndices();
    std::vector<std::string> buffer = getDocument()->getBuffer();

    if (selectedIndices[0][0] == -1) {return;}
    if (selectedIndices[0][0] < this->currentTopLine) {
        selectedIndices[0][0] = this->currentTopLine;
        selectedIndices[0][1] = 0;
    }
    if (selectedIndices[1][0] >= this->currentTopLine + LINES-linesToRemove) {
        selectedIndices[1][0] = this->currentTopLine + LINES-linesToRemove;
        selectedIndices[1][1] = buffer[this->currentTopLine + LINES-linesToRemove].length();
    }
    selectedIndices[0][0] -= this->currentTopLine;
    selectedIndices[1][0] -= this->currentTopLine;
    wmove(this->window, selectedIndices[0][0], selectedIndices[0][1]);
    if (selectedIndices[0][0] == selectedIndices[1][0]) {
        wchgat(this->window, selectedIndices[1][1]-selectedIndices[0][1], A_STANDOUT, 0, NULL);
    }
    else {
        int currentx = selectedIndices[0][1];
        for (int i = selectedIndices[0][0]; i < selectedIndices[1][0]; i++) {
            int length = buffer[i+this->currentTopLine].length() - currentx;
            wchgat(this->window, length, A_STANDOUT, 0, NULL);
            wmove(this->window, i+1, 0);
            currentx = 0;
        }
        wchgat(this->window, selectedIndices[1][1], A_STANDOUT, 0, NULL);
    }
}

void RenderingHandler::renderCommand(const char* command, const char* directions) {
    this->renderDoc(3);
    this->renderSelected(3);
    wmove(this->window, LINES-2, 0);
    wprintw(this->window, "%s", directions);
    wmove(this->window, LINES-1, 0);
    wprintw(this->window, "%s", command);
    wmove(this->window, LINES-1, static_cast<int>(strlen(command)));
}

void RenderingHandler::displayHighlightedSearchTerm(std::string searchTerm, int& cursorXPos, int& cursorYPos, int line, int index) {
    const char* description = "Press enter to see next, or esc to exit.";
    const char* searchTermcStr = searchTerm.c_str();
    std::vector<std::string> buffer = getDocument()->getBuffer();
    if (line < (LINES-4)/2) {
        this->currentTopLine = 0;
        cursorYPos = line;
        cursorXPos = index;
    }
    else if (line > static_cast<int>(buffer.size())-((LINES-4)/2)) {
        this->currentTopLine = static_cast<int>(buffer.size()) - (LINES-3);
        cursorYPos = (LINES-3) - (static_cast<int>(buffer.size())-line);
        cursorXPos = index;
    }
    else {
        this->currentTopLine = line - ((LINES-4)/2);
        cursorYPos = (LINES-4)/2;
        cursorXPos = index;
    }
    renderCommand(searchTermcStr, description);
    int length = searchTerm.length();
    wmove(this->window, cursorYPos, cursorXPos);
    wchgat(this->window, length, A_STANDOUT, 0, NULL);	
    cursorXPos += length;
    wmove(this->window, cursorYPos, cursorXPos);
}

void RenderingHandler::initiallyResize() {
    this->document->resizeTextBuffer(COLS);
}
