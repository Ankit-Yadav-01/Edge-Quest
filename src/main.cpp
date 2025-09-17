#include <iostream>
#include "Edge-Quest.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace std;

extern "C" {

// Initialize the game with a given grid size
EMSCRIPTEN_KEEPALIVE
void startGame(int gridSize) {
    n = gridSize;
    initGrid(n);
}

// Get the current grid as a string (for JS to render)
EMSCRIPTEN_KEEPALIVE
const char* getGrid() {
    static string gridStr;
    gridStr = getFullGridString();
    return gridStr.c_str();
}

// Get instructions as a string
EMSCRIPTEN_KEEPALIVE
const char* getInstructions() {
    static string instr;
    instr = getInstructionsString();
    return instr.c_str();
}

// Process a move; returns 0 if game over, 1 if continue, 2 if win
EMSCRIPTEN_KEEPALIVE
EMSCRIPTEN_KEEPALIVE
int processMove(char moveChar) {
    int res = playerMove(moveChar);
    if (res == -1) return 0; // Game over
    if (res == 1) return 2;  // Win
    return 1;                 // Continue
}


// Get the current direction as a string
EMSCRIPTEN_KEEPALIVE
const char* getCurrentDirection() {
    return getCurrentDirectionImpl();
}

EMSCRIPTEN_KEEPALIVE
const char* getHint() {
    return getHintImpl();
}

}
