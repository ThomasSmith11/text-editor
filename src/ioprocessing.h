#ifndef ioProcessor
#define ioProcessor

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <cstring>
#include <ncurses.h>

using namespace std;

namespace io {
    class processor {

        private:
            const char* filename;
            WINDOW *window;
            vector<string> textBuffer;
            int currentTopLine = 0;
            
            void saveToFile() {
                ofstream outputFile(this->filename);
                if (outputFile.is_open()) {
                    for (const string& line : this->textBuffer) {
                        outputFile << line << "\n";
                    }
                    outputFile.close();
                } else {
                    wprintw(this->window, "Error saving file");
                    wrefresh(this->window);
                }
            }
            
            void renderDoc() {
                wclear(this->window);
                int i = 0;
                while (i < LINES-3) {
                    if (i >= static_cast<int>(this->textBuffer.size())) {
                        break;
                    }
                    string line = this->textBuffer[this->currentTopLine+i];
                    line+="\n";
                    const char* cStr = new char[line.length() + 1];
                    strcpy(const_cast<char*>(cStr), line.c_str());
                    wprintw(this->window, "%s", cStr);
                    i++;
                }
                wrefresh(this->window);
            }

            string collectCommandInput() {
                string input;
                int nextChar;
                nextChar = wgetch(this->window);
                while (nextChar != 10) {
                    if (nextChar == KEY_BACKSPACE && !input.empty()) {
                        input.pop_back();
                    }
                    else if (nextChar != KEY_BACKSPACE) {
                        input += static_cast<char>(nextChar);
                    }
                    displayCommand(input);
                    nextChar = wgetch(this->window);
                }
                return input;
            }
            
            void displayCommand(string command) {
                string whitespace(command.length()+1, ' ');

                const char* cStr = new char[command.length() + 1];
                strcpy(const_cast<char*>(cStr), command.c_str());
                const char* whitespaceStr = new char[whitespace.length() + 1];
                strcpy(const_cast<char*>(whitespaceStr), whitespace.c_str());

                wmove(this->window, LINES-2, 0);
                wprintw(this->window, "%s", whitespaceStr);
                wmove(this->window, LINES-2, 0);
                wprintw(this->window, "%s", cStr);
                wmove(this->window, LINES-2, command.length());
            }
            
            void processFind(int& x, int& y) {
                wmove(this->window, LINES-2, 0);
                wprintw(this->window, "  ");
                wmove(this->window, LINES-3, 0);
                wprintw(this->window, "Type value to search for, then enter: ");
                wmove(this->window, LINES-2, 0);
                string searchTerm = collectCommandInput();
                wmove(this->window, LINES-3, 0);
                wprintw(this->window, "Press enter to see next, or esc to exit.");
                doSearch(searchTerm, x, y);
                cleanupCommandInputArea();
            }
            
            vector<vector<int>> getMatches(regex expression){
                int currentLine = 0;
                vector<vector<int>> matches;
                for (string line : this->textBuffer) {
                    smatch m;
                    while (regex_search(line, m, expression)) {
                        int index = m.position();
                        vector<int> match;
                        match.push_back(currentLine);
                        match.push_back(index);
                        matches.push_back(match);
                        line = m.suffix().str();
                    }
                    currentLine++;
                }
                return matches;
            }
        
            void doSearch(string searchTerm, int& x, int& y) {
                const regex expression(searchTerm, regex_constants::icase);
                vector<vector<int>> matches = getMatches(expression);
                
                int nextChar;
                while (TRUE) {
                    for (vector match : matches) {
                        while (nextChar != 10 && nextChar != 27) {
                            nextChar = wgetch(this->window);
                        }
                        if (nextChar == 27) {
                            break;
                        }
                        int line = match[0];
                        int index = match[1];
                        if (line < (LINES-4)/2) {
                            this->currentTopLine = 0;
                            y = line;
                            x = index;
                        }
                        else if (line > static_cast<int>(this->textBuffer.size())-((LINES-4)/2)) {
                            this->currentTopLine = static_cast<int>(this->textBuffer.size()) - (LINES-3);
                            y = (LINES-3) - (static_cast<int>(this->textBuffer.size())-line);
                            x = index;
                        }
                        else {
                            this->currentTopLine = line - ((LINES-4)/2);
                            y = (LINES-4)/2;
                            x = index;
                        }
                        displayHighlightedSearch(searchTerm, x, y);
                        nextChar = 0;
                    }
                    if (nextChar == 27) {
                        break;
                    }
                }
                wchgat(this->window, searchTerm.length(), A_NORMAL, 0, NULL);	
            }
        
            void displayHighlightedSearch(string searchTerm, int& x, int& y) {
                int length = searchTerm.length();
                renderDoc();
                wmove(this->window, LINES-3, 0);
                wprintw(this->window, "Press enter to see next, or esc to exit.");
                displayCommand(searchTerm);
                wmove(this->window, y, x);
                wchgat(this->window, length, A_STANDOUT, 0, NULL);	
            }

            void processCommand(string command, int& x, int& y) {
                if (command == ":q"){
                    delwin(this->window);
                    endwin();
                    exit(0);
                }
                if (command == ":qs" || command == ":sq"){
                    saveToFile();
                    delwin(this->window);
                    endwin();
                    exit(0);
                }
                if (command == ":s"){
                    saveToFile();
                    wmove(this->window, LINES-2, 0);
                    wprintw(this->window, "  ");
                }
                if (command == ":f"){
                    processFind(x, y);
                }
                cleanupCommandInputArea();
            }
        
            void cleanupCommandInputArea() {
                wmove(this->window, LINES-2, 0);
                string whitespace(COLS, ' ');
                const char* whitespaceStr = new char[whitespace.length() + 1];
                strcpy(const_cast<char*>(whitespaceStr), whitespace.c_str());
                wprintw(this->window, "%s", whitespaceStr);
                wmove(this->window, LINES-3, 0);
                wprintw(this->window, "%s", whitespaceStr);
            }
            

        public:
            static string emptyString;
        
            processor(const char* fname, WINDOW *win, vector<string> tB) : filename(fname), window(win), textBuffer(tB){}

            void populateTextBufferAndInitiallyRender() {
                ifstream inputFile(this->filename);
                if (!inputFile.is_open()) {
                    wprintw(this->window, "Error opening file");
                }
                if (inputFile.is_open()) {
                    string line;
                    while (getline(inputFile, line)) {
                        this->textBuffer.push_back(line);
                    }
                    inputFile.close();
                }
                if (this->textBuffer.size() == 0) {
                    this->textBuffer.push_back(emptyString);
                }
                renderDoc();
            }

            void processArrowKey(int key, int& x, int& y) {
                if (key == KEY_UP) {
                    y--;
                    if (y<0 && this->currentTopLine != 0) {
                        this->currentTopLine--;
                        renderDoc();
                    }
                } else if (key == KEY_DOWN) {
                    y++;
                    if (y>LINES-4 && this->currentTopLine+LINES-4 < this->textBuffer.size()-1) {
                        this->currentTopLine++;
                        renderDoc();
                    }
                } else if (key == KEY_RIGHT) {
                    x++;
                } else if (key == KEY_LEFT) {
                    x--;
                }

                y = max(0, min(static_cast<int>(this->textBuffer.size())-1, min(y, LINES-4)));
                x = max(0, min(x, static_cast<int>(this->textBuffer[y+this->currentTopLine].size())));
                wmove(this->window, y, x);
                wrefresh(this->window);
            }

            void processReturn(int& x, int& y) {
                bool cursorAtEndOfLine = (static_cast<int>(this->textBuffer[y + this->currentTopLine].size()) == x);
                if (cursorAtEndOfLine) {
                    y++;
                    x = 0;
                    this->textBuffer.insert(this->textBuffer.begin() + y + this->currentTopLine, emptyString);
                    if (y>LINES-4 && this->currentTopLine+LINES-4 != this->textBuffer.size()-1) {
                        this->currentTopLine++;
                        y=LINES-4;
                    }
                    renderDoc();
                    wmove(this->window, y, x);
                }
                else {
                    string substringOne = this->textBuffer[y+this->currentTopLine].substr(0,x);
                    string substringTwo = this->textBuffer[y+this->currentTopLine].substr(x, static_cast<int>(this->textBuffer[y + this->currentTopLine].size()));
                    this->textBuffer[y + this->currentTopLine] = substringOne;
                    y++;
                    x=0;
                    this->textBuffer.insert(this->textBuffer.begin() + y + this->currentTopLine, substringTwo);
                    if (y>LINES-4) {
                        this->currentTopLine++;
                        y=LINES-4;
                    }
                    renderDoc();
                    wmove(this->window, y, x);
                }
            }
            
            void processDelete(int& x, int& y) {
                if (x != 0) {
                    this->textBuffer[y + this->currentTopLine].erase(x-1, 1);
                    x--;
                    renderDoc();
                    wmove(this->window, y, x);
                }
                else if (x == 0 && y != 0) {
                    string line = this->textBuffer[y + this->currentTopLine];
                    this->textBuffer.erase(this->textBuffer.begin() + y + this->currentTopLine);
                    y--;
                    x = static_cast<int>(this->textBuffer[y+ this->currentTopLine].size());
                    this->textBuffer[y + this->currentTopLine] += line;
                    renderDoc();
                    wmove(this->window, y, x);
                }
                else if (x == 0 && y == 0 && this->currentTopLine != 0) {
                    string line = this->textBuffer[y + this->currentTopLine];
                    this->textBuffer.erase(this->textBuffer.begin() + y + this->currentTopLine);
                    this->currentTopLine--;
                    x = static_cast<int>(this->textBuffer[y + this->currentTopLine].size());
                    this->textBuffer[y + this->currentTopLine] += line;
                    renderDoc();
                    wmove(this->window, y, x);
                }
            }
            
            void processTab(int& x, int& y) {
                wprintw(this->window, "Recieved tab input");
                this->textBuffer[y + this->currentTopLine].insert(x, 4, ' ');
                x += 4;
                renderDoc();
                wmove(this->window, y, x);
            }
            
            void processNormalKey(int key, int& x, int& y) {
                char character = static_cast<char>(key);
                this->textBuffer[y+this->currentTopLine].insert(x, 1, character);
                x++;
                renderDoc();
                wmove(this->window, y, x);
            }
            
            void processEscapeSequence(int& x, int& y) {
                string command = collectCommandInput();
                processCommand(command, x, y);
            }
    };
    string processor::emptyString = "";
}

#endif

