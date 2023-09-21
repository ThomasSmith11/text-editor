#pragma once

#include <ncurses.h>
#include "document.h"
#include <vector>

class RenderingHandler {
private:
    int currentTopLine;
    Document* document;
    WINDOW *window;
    static RenderingHandler* instance;
    RenderingHandler();
public:
    ~RenderingHandler();
    int& getCurrentTopLine();
    static RenderingHandler* getInstance();
    void setDocument(Document* document);
    Document* getDocument();
    void setWindow(WINDOW *window);
    WINDOW* getWindow();
    void moveCursor(int cursorXpos, int cursorYpos);
    void renderDoc(int linesToRemove = 0);
    void renderSelected(int linesToRemove = 0);
    void renderCommand(const char* command, const char* directions = "");
    void displayHighlightedSearchTerm(std::string searchTerm, int& cursorXPos, int& cursorYPos, int line, int index);
};
