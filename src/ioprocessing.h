#ifndef ioProcessor
#define ioProcessor

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
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

            void processCommand(string command) {
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
                    wprintw(this->window, "               ");
                }              
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
                    if (y>LINES-4 && this->currentTopLine+LINES-4 != this->textBuffer.size()-1) {
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
                bool cursorAtEndOfLine = (static_cast<int>(this->textBuffer[y].size()) == x);
                if (cursorAtEndOfLine) {
                    y++;
                    x = 0;
                    this->textBuffer.insert(this->textBuffer.begin() + y, emptyString);
                    renderDoc();
                    wmove(this->window, y, x);
                }
                else {
                    string substringOne = this->textBuffer[y].substr(0,x);
                    string substringTwo = this->textBuffer[y].substr(x, static_cast<int>(this->textBuffer[y].size()));
                    this->textBuffer[y] = substringOne;
                    y++;
                    x=0;
                    this->textBuffer.insert(this->textBuffer.begin() + y, substringTwo);
                    renderDoc();
                    wmove(this->window, y, x);
                }
            }
            
            void processDelete(int& x, int& y) {
                if (x != 0) {
                    this->textBuffer[y].erase(x-1, 1);
                    x--;
                    renderDoc();
                    wmove(this->window, y, x);
                }
                else if (x == 0 && y != 0) {
                    string line = this->textBuffer[y];
                    this->textBuffer.erase(this->textBuffer.begin() + y);
                    y--;
                    x = static_cast<int>(this->textBuffer[y].size());
                    this->textBuffer[y] += line;
                    renderDoc();
                    wmove(this->window, y, x);
                }
            }
            
            void processTab(int& x, int& y) {
                wprintw(this->window, "Recieved tab input");
                this->textBuffer[y].insert(x, 4, ' ');
                x += 4;
                renderDoc();
                wmove(this->window, y, x);
            }
            
            void processNormalKey(int key, int& x, int& y) {
                char character = static_cast<char>(key);
                this->textBuffer[y].insert(x, 1, character);
                x++;
                renderDoc();
                wmove(this->window, y, x);
            }
            
            void processEscapeSequence() {
                string command;
                int nextChar;
                nextChar = wgetch(this->window);
                while (nextChar != 10) {
                    if (nextChar == KEY_BACKSPACE && !command.empty()) {
                        command.pop_back();
                    }
                    else if (nextChar != KEY_BACKSPACE) {
                        command += static_cast<char>(nextChar);
                    }
                    displayCommand(command);
                    nextChar = wgetch(this->window);
                }
                processCommand(command);
            }
    };
    string processor::emptyString = "";
}

#endif

