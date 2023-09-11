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
            if (line.empty()) {
                this->textBuffer.push_back("\n");
            }
            else {
                line += "\n";
                this->textBuffer.push_back(line);
            }
        }
        inputFile.close();
    }
    if (this->textBuffer.size() == 0) {
        this->textBuffer.push_back("\n");
    }
}

void Document::saveDoc() const {
    std::ofstream outputFile(this->filename);
    if (outputFile.is_open()) {
        for (const std::string& line : this->textBuffer) {
            outputFile << line;
        }
        outputFile.close();
    }
}

std::vector<std::string>& Document::getBuffer() {
    return this->textBuffer;
}

int Document::getLineLength(int lineNum) {
    if (this->textBuffer[lineNum].back() == '\n') {
        return this->textBuffer[lineNum].length() - 1;
    }
    return this->textBuffer[lineNum].length();
}

std::vector<std::string> Document::splitString(std::string stringToSplit, int size) {
    std::vector<std::string> returnVec;
    while (!stringToSplit.empty()) {
        if (stringToSplit.length() <= size) {
            returnVec.push_back(stringToSplit);
            stringToSplit.clear();
            break;
        }
        std::string substring = stringToSplit.substr(0, size);
        size_t lastIndex = substring.find_last_of(' ');
            if (lastIndex == std::string::npos) {
                lastIndex = size;
            }
            else {
                lastIndex+=1;
            }
        std::string realSubstring = stringToSplit.substr(0, lastIndex);
        returnVec.push_back(realSubstring);
        stringToSplit.erase(0, lastIndex);
    }
    return returnVec;
}

void Document::resizeTextBuffer(int cols) {
    int numBufferLines = this->textBuffer.size();
    std::vector<std::string> tempBuffer;
    std::string line = "";
    for (int i = 0; i < numBufferLines; i++) {
        line += this->textBuffer[i];
        if (line.back() == '\n') {
            tempBuffer.push_back(line);
            line = "";
        }
    }

    cols = cols-1;
    int currentOffset = 0;
    int numTempLines = tempBuffer.size();
    for (int i = 0; i < numTempLines; i++) {
        std::string line = tempBuffer[i+currentOffset];
        if (line.length() > cols) {
            std::vector<std::string> splitStrings = splitString(line, cols);
            tempBuffer[i+currentOffset] = splitStrings[0];
            for (int j = 1; j < splitStrings.size(); j++) {
                currentOffset++;
                tempBuffer.insert(tempBuffer.begin() + i + currentOffset, splitStrings[j]);
            }
        }
    }
    this->textBuffer = tempBuffer;
}
