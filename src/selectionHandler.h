#pragma once

#include <vector>
#include <string>

class SelectionHandler {
private:
    static std::vector<std::vector<int>> selectedIndices;
    static void calcUpIndices(int cursorXpos, int cursorYpos);
    static void calcDownIndices(int cursorXpos, int cursorYpos);
    static void calcRightIndices(int cursorXpos, int cursorYpos);
    static void calcLeftIndices(int cursorXpos, int cursorYpos);

public:
    static const std::vector<std::vector<int>>& getSelectedIndices();
    static void updateSelectedIndices(int cursorXpos, int cursorYpos, std::string direction);
    static void initializeSelectedIndices();
};
