#include <iostream>
#include <vector>
#include "document.h"
#include "inputHandler.h"
#include "renderingHandler.h"
#include "selectionHandler.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const char* filename = argv[1];

    Document doc = Document(filename);
    doc.openDoc();
    RenderingHandler* renderer = RenderingHandler::getInstance();
    renderer->setDocument(&doc);
    renderer->renderDoc();

    int cursorXPos = 0;
    int cursorYPos = 0;
    renderer->moveCursor(cursorYPos, cursorXPos);
    InputHandler::initializeKeyDefinitions();
    SelectionHandler::initializeSelectedIndices();
    int key;
    
    while (TRUE) {
        key = InputHandler::collectInput();
        InputHandler::processKeyInput(key, cursorXPos, cursorYPos);
        renderer->renderDoc();
        if (InputHandler::selecting) {
            renderer->renderSelected();
        }
        renderer->moveCursor(cursorXPos, cursorYPos);
    }
    return 0;
}
