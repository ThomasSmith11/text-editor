#pragma once

#include <string>
#include <vector>
#include <ncurses.h>

class Document {
public:
    Document(const char* filename);
    void openDoc();
    void saveDoc() const;
    std::vector<std::string>& getBuffer();
    void resizeTextBuffer(int cols);
    int getLineLength(int lineNum);
    
private:
    const char* filename;
    std::vector<std::string> textBuffer;
    std::vector<std::string> splitString(std::string stringToSplit, int size);
};
