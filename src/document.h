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
    int getLength() const;
private:
    const char* filename;
    std::vector<std::string> textBuffer;
};
