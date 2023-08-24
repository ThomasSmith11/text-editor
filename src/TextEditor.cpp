#include <iostream>
#include <vector>
#include <ncurses.h>
#include "ioprocessing.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const char* filename = argv[1];
    
    initscr();
    cbreak();

    WINDOW *textwin = newwin(LINES-1, COLS, 0, 0);
    keypad(textwin, TRUE);
    noecho();
    ESCDELAY = 0;
    
    std::vector<std::string> fileState;

    io::processor processor(filename, textwin, fileState);
    processor.populateTextBufferAndInitiallyRender();
    
    int cursorXPos = 0;
    int cursorYPos = 0;
    wmove(textwin, cursorYPos, cursorXPos);
    
    int ch;

    while (true) {
        ch = wgetch(textwin);
        if (ch == 27) {
            wmove(textwin, LINES-2, 0);
            wrefresh(textwin);
            processor.processEscapeSequence();
        }
        else if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
            processor.processArrowKey(ch, cursorXPos, cursorYPos);
        }
        else if (ch == 10) {
            processor.processReturn(cursorXPos, cursorYPos);
        }
        else if (ch == KEY_BACKSPACE) {
            processor.processDelete(cursorXPos, cursorYPos);
        }
        else {
            processor.processNormalKey(ch, cursorXPos, cursorYPos);
        }
    }
    return 0;
}
