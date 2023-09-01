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
    int KEY_SUP = KEY_MAX + 1; define_key("\E[1;2A", KEY_SUP);
    int KEY_SDOWN = KEY_MAX + 2; define_key("\E[1;2B", KEY_SDOWN);
    
    
    
    std::vector<std::string> fileState;

    io::processor processor(filename, textwin, fileState);
    processor.populateTextBufferAndInitiallyRender();
    
    int cursorXPos = 0;
    int cursorYPos = 0;
    wmove(textwin, cursorYPos, cursorXPos);
    
    int ch;

    while (true) {
        ch = wgetch(textwin);
        
    }
    return 0;
}
