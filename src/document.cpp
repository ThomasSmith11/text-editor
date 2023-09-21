#include "document.h"
#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

Document::Document(const char* fname) : filename(fname) {}

void Document::openDoc() {
    std::ifstream inputFile(this->filename);
    if (!inputFile.is_open()) {
        std::cout << "Failed to open document" << std::endl;
        exit(1);
    }
    if (inputFile.is_open()) {
        std::string line;
        while (getline(inputFile, line)) {
            this->textBuffer.push_back(line);
        }
        inputFile.close();
    }
    if (this->textBuffer.size() == 0) {
        this->textBuffer.push_back("");
    }
}

void Document::saveDoc() const {
    std::ofstream outputFile(this->filename);
    if (outputFile.is_open()) {
        for (const std::string& line : this->textBuffer) {
            outputFile << line << "\n";
        }
        outputFile.close();
    }
}

std::vector<std::string>& Document::getBuffer() {
    return this->textBuffer;
}

int Document::getLength() const {
    return this->textBuffer.size();
}
