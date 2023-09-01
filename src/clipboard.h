#include <string>
#include <vector>

class Clipboard {
public:
    static void cut(int& cursorXPos, int&cursorYPos);
    static void copy();
    static void paste(int& cursorXPos, int&cursorYPos);
private:
    static std::vector<std::string> clipboard;
    static void doCopy();
};
