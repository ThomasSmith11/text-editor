#include "selectionHandler.h"
#include "renderingHandler.h"
#include <vector>
#include <string>

std::vector<std::vector<int>> SelectionHandler::selectedIndices;

void SelectionHandler::calcUpIndices(int x, int y) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int newy;
    int newx;
    int& currentTopLine = renderer->getCurrentTopLine();
    int faketopline = currentTopLine;
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (selectedIndices[0][0] == -1) {
        selectedIndices[1][0] = y + faketopline;
        selectedIndices[1][1] = x + faketopline;
        newx = x;
        newy = y - 1;
        if (newy<0 && faketopline != 0) {
            faketopline--;
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[0][0] = newy + faketopline;
        selectedIndices[0][1] = newx + faketopline;
    }
    else if (y == selectedIndices[0][0] && x == selectedIndices[0][1]) {
        newx = x;
        newy = y - 1;
        if (newy<0 && faketopline != 0) {
            faketopline--;
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[0][0] = newy + faketopline;
        selectedIndices[0][1] = newx + faketopline;
    }
    else {
        newx = x;
        newy = y - 1;
        if (newy<0 && faketopline != 0) {
            faketopline--;
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-4)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[1][0] = newy + faketopline;
        selectedIndices[1][1] = newx + faketopline;
    }
    //reset selected indices if nothing selected
    if (selectedIndices[0][0] == selectedIndices[1][0] && selectedIndices[0][1] == selectedIndices[1][1]) {
        initializeSelectedIndices();
    }
}

void SelectionHandler::calcDownIndices(int x, int y) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int newy;
    int newx;
    int& currentTopLine = renderer->getCurrentTopLine();
    int faketopline = currentTopLine;
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (selectedIndices[0][0] == -1) {
        selectedIndices[0][0] = y + faketopline;
        selectedIndices[0][1] = x + faketopline;
        newx = x;
        newy = y + 1;
        if (newy>LINES-1 && faketopline+LINES-1 < buffer.size()-1) {
            faketopline++;
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[1][0] = newy + faketopline;
        selectedIndices[1][1] = newx + faketopline;
    }
    else if (y == selectedIndices[1][0] && x == selectedIndices[1][1]) {
        newx = x;
        newy = y + 1;
        if (newy>LINES-1 && faketopline+LINES-1 < buffer.size()-1) {
            faketopline++;
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[1][0] = newy + faketopline;
        selectedIndices[1][1] = newx + faketopline;
    }
    else {
        newx = x;
        newy = y + 1;
        if (newy>LINES-1 && faketopline+LINES-1 < buffer.size()-1) {
            faketopline++;
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[0][0] = newy + faketopline;
        selectedIndices[0][1] = newx + faketopline;
    }
    //reset selected indices if nothing selected
    if (selectedIndices[0][0] == selectedIndices[1][0] && selectedIndices[0][1] == selectedIndices[1][1]) {
        initializeSelectedIndices();
    }
}

void SelectionHandler::calcRightIndices(int x, int y) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int newy;
    int newx;
    int& currentTopLine = renderer->getCurrentTopLine();
    int faketopline = currentTopLine;
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (selectedIndices[0][0] == -1) {
        selectedIndices[0][0] = y + faketopline;
        selectedIndices[0][1] = x + faketopline;
        newx = x+1;
        newy = y;
        if (newx>buffer[newy+faketopline].length() && newy+faketopline != buffer.size()-1) {
            newy++;
            newx=0;
            if (newy>LINES-1 && faketopline+LINES-1 < buffer.size()-1) {
                faketopline++;
            }
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[1][0] = newy + faketopline;
        selectedIndices[1][1] = newx + faketopline;
    }
    else if (y == selectedIndices[1][0] && x == selectedIndices[1][1]) {
        newx = x+1;
        newy = y;
        if (newx>buffer[newy+faketopline].length() && newy+faketopline != buffer.size()-1) {
            newy++;
            newx=0;
            if (newy>LINES-1 && faketopline+LINES-1 < buffer.size()-1) {
                faketopline++;
            }
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[1][0] = newy + faketopline;
        selectedIndices[1][1] = newx + faketopline;
    }
    else {
        newx = x+1;
        newy = y;
        if (newx>buffer[newy+faketopline].length() && newy+faketopline != buffer.size()-1) {
            newy++;
            newx=0;
            if (newy>LINES-1 && faketopline+LINES-1 < buffer.size()-1) {
                faketopline++;
            }
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[0][0] = newy + faketopline;
        selectedIndices[0][1] = newx + faketopline;
    }
    //reset selected indices if nothing selected
    if (selectedIndices[0][0] == selectedIndices[1][0] && selectedIndices[0][1] == selectedIndices[1][1]) {
        initializeSelectedIndices();
    }
}

void SelectionHandler::calcLeftIndices(int x, int y) {
    RenderingHandler* renderer = RenderingHandler::getInstance();
    int newy;
    int newx;
    int& currentTopLine = renderer->getCurrentTopLine();
    int faketopline = currentTopLine;
    std::vector<std::string>& buffer = renderer->getDocument()->getBuffer();
    if (selectedIndices[0][0] == -1) {
        selectedIndices[1][0] = y + faketopline;
        selectedIndices[1][1] = x + faketopline;
        newx = x-1;
        newy = y;
        if (newx<0 && (faketopline != 0 || newy != 0)) {
            newy--;
            newx=buffer[newy+faketopline].length();
            if (newy<0 && faketopline != 0) {
                faketopline--;
            }
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[0][0] = newy + faketopline;
        selectedIndices[0][1] = newx + faketopline;
    }
    else if (y == selectedIndices[0][0] && x == selectedIndices[0][1]) {
        newx = x-1;
        newy = y;
        if (newx<0 && (faketopline != 0 || newy != 0)) {
            newy--;
            newx=buffer[newy+faketopline].length();
            if (newy<0 && faketopline != 0) {
                faketopline--;
            }
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[0][0] = newy + faketopline;
        selectedIndices[0][1] = newx + faketopline;
    }
    else {
        newx = x-1;
        newy = y;
        if (newx<0 && (faketopline != 0 || newy != 0)) {
            newy--;
            newx=buffer[newy+faketopline].length();
            if (newy<0 && faketopline != 0) {
                faketopline--;
            }
        }
        newy = std::max(0, std::min(static_cast<int>(buffer.size())-1, std::min(newy, LINES-1)));
        newx = std::max(0, std::min(newx, static_cast<int>(buffer[newy+faketopline].size())));
        selectedIndices[1][0] = newy + faketopline;
        selectedIndices[1][1] = newx + faketopline;
    }
    if (selectedIndices[0][0] == selectedIndices[1][0] && selectedIndices[0][1] == selectedIndices[1][1]) {
        initializeSelectedIndices();
    }
}

const std::vector<std::vector<int>>& SelectionHandler::getSelectedIndices() {
    return selectedIndices;
}

void SelectionHandler::updateSelectedIndices(int x, int y, std::string direction) {
    if (direction == "up") {
        calcUpIndices(x, y);
    }
    else if (direction == "down") {
        calcDownIndices(x, y);
    }
    else if (direction == "right") {
        calcRightIndices(x, y);
    }
    else if (direction == "left") {
        calcLeftIndices(x, y);
    }
}

void SelectionHandler::initializeSelectedIndices() {
    selectedIndices.clear();
    std::vector<int> initialize;
    initialize.push_back(-1);
    initialize.push_back(-1);
    selectedIndices.push_back(initialize);
    selectedIndices.push_back(initialize);
}

