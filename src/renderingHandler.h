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
    int& getCurrentTopLine();
    static RenderingHandler* getInstance();
    void setDocument(Document* document);
    Document* getDocument();
    void setWindow(WINDOW *window);
    WINDOW* getWindow();
    void moveCursor(int cursorXpos, int cursorYpos);
    void renderDoc(int linesToRemove = 0);
    void renderSelected(std::vector<std::vector<int>> selectedIndices);
    void closeAndDeleteWindow();
    void renderCommand(const char* command, const char* directions = "");
};
