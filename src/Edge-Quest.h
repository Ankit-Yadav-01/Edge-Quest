#pragma once
#include <string>
#include <utility>

// Core functions
void initGrid(int n);

std::string getFullGridString();
std::string getInstructionsString();
const char* getCurrentDirectionImpl();
const char* getHintImpl();
int playerMove(char moveChar);
void flipKNeighborsToWrong(int x, int y, int k);
std::pair<char, char> hintTwoDirectionsToKey();

// Globals (defined in Edge-Quest.cpp)
extern int n;
extern int correctCount;
extern char correctDir;
