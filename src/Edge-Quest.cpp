#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include "Edge-Quest.h"


using namespace std;

struct Cell {
    char dir;
    bool correct;
};

int n;
vector<vector<Cell>> grid;
int correctCount = 0;
int px, py;
char correctDir;
int powerLeft;
int hintsLeft;
int keyx = -1, keyy = -1;
bool hasKey = false;


char randWrongDir() {
    string d = "LRUD";
    char c;
    do { c = d[rand() % 4]; } while (c == correctDir);
    return c;
}

void initGrid(int n_) {
    n = n_;

    grid.assign(n, vector<Cell>(n));
    correctCount = 0;

    string d = "LRUD";
    correctDir = d[rand() % 4];
    cout << "Correct exit direction: " << correctDir << endl;

    int total = n * n;
    int numCorrect = total / 4;
    vector<int> indices(total);
    for (int i = 0; i < total; i++) indices[i] = i;

    random_device rd;
    mt19937 rng(rd());
    shuffle(indices.begin(), indices.end(), rng);

    for (int k = 0; k < numCorrect; k++) {
        int idx = indices[k];
        int i = idx / n, j = idx % n;
        grid[i][j].dir = correctDir;
        grid[i][j].correct = true;
        correctCount++;
    }
    for (int k = numCorrect; k < total; k++) {
        int idx = indices[k];
        int i = idx / n, j = idx % n;
        grid[i][j].dir = randWrongDir();
        grid[i][j].correct = false;
    }

    px = py = n / 2;
    powerLeft = (n + 1) / 2;
    hintsLeft = (n + 1) / 2;

    vector<int> freeIdx;
    for (int i = 0; i < total; i++) {
        int r = i / n, c = i % n;
        if (!(r == px && c == py)) freeIdx.push_back(i);
    }
    int choose = freeIdx[rand() % freeIdx.size()];
    keyx = choose / n;
    keyy = choose % n;
    hasKey = false;
}

vector<pair<int, int>> getNeighbors(int x, int y) {
    vector<pair<int, int>> res;
    if (x > 0) res.push_back({x - 1, y});
    if (x < n - 1) res.push_back({x + 1, y});
    if (y > 0) res.push_back({x, y - 1});
    if (y < n - 1) res.push_back({x, y + 1});
    return res;
}

bool isVisible(int x, int y) {
    return abs(x - px) <= 1 && abs(y - py) <= 1;
}

void flipOneNeighborToCorrect(int x, int y) {
    vector<pair<int, int>> neigh = getNeighbors(x, y);
    vector<pair<int, int>> incorrects;
    for (auto p : neigh) if (!grid[p.first][p.second].correct) incorrects.push_back(p);
    if (incorrects.empty()) return;
    int idx = rand() % incorrects.size();
    int nx = incorrects[idx].first, ny = incorrects[idx].second;
    grid[nx][ny].correct = true;
    grid[nx][ny].dir = correctDir;
    correctCount++;
}

void flipKNeighborsToWrong(int x, int y, int k) {
    vector<pair<int, int>> neigh = getNeighbors(x, y);
    if (neigh.empty()) return;
    vector<int> idxs(neigh.size());
    iota(idxs.begin(), idxs.end(), 0);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(idxs.begin(), idxs.end(), rng);
    int take = min(k, (int)neigh.size());
    for (int t = 0; t < take; t++) {
        int iidx = idxs[t];
        int nx = neigh[iidx].first, ny = neigh[iidx].second;
        if (grid[nx][ny].correct) {
            grid[nx][ny].correct = false;
            correctCount--;
            grid[nx][ny].dir = randWrongDir();
        } else {
            char old = grid[nx][ny].dir, nw;
            do { nw = randWrongDir(); } while (nw == old);
            grid[nx][ny].dir = nw;
        }
    }
}

string getFullGridString() {
    ostringstream oss;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == px && j == py) {
                oss << "P ";
            } else if (isVisible(i, j)) {
                oss << grid[i][j].dir << " ";
            } else {
                oss << "# ";
            }
        }
        oss << '\n';
    }
    oss << "Your cell direction: " << grid[px][py].dir << "\n";
    if (hasKey) oss << "You have the key! Now find the exit.\n";
    else oss << "You need to find the key first.\n";
    oss << "Power: " << powerLeft << "   Hints left: " << hintsLeft << "\n";
    return oss.str();
}

string getInstructionsString() {
    ostringstream oss;
    oss << "=====================================\n";
    oss << "          MAZE GAME INSTRUCTIONS      \n";
    oss << "=====================================\n";
    oss << "1. You are in a maze of size n x n (odd n). current n = " << n << "\n";
    oss << "2. Each cell has a hidden direction (L/R/U/D).\n";
    oss << "3. Only your current cell and its 8 neighbors are visible.\n";
    oss << "4. Player is marked as 'P'.\n";
    oss << "5. Directions of neighbors are displayed, all other cells are hidden (#).\n";
    oss << "6. You can move using U (up), D (down), L (left), R (right).\n";
    oss << "7. Only one global correct direction exists in the maze.\n";
    oss << "8. You must find the key before exiting.\n";
    oss << "9. Exit is on the edge in the correct global direction.\n";
    oss << "10. Wrong moves cost power. Power < 0 = GAME OVER.\n";
    oss << "11. You have hints to locate the key.\n";
    return oss.str();
}

pair<char, char> hintTwoDirectionsToKey() {
    if (keyx == -1 && keyy == -1) return {'N', 'N'};
    int dx = keyx - px, dy = keyy - py;
    char vertical = 'N', horizontal = 'N';
    if (dx < 0) vertical = 'U'; else if (dx > 0) vertical = 'D';
    if (dy < 0) horizontal = 'L'; else if (dy > 0) horizontal = 'R';
    if (dx == 0 && dy == 0) return {'N', 'N'};
    else if (dx == 0) return {horizontal, horizontal};
    else if (dy == 0) return {vertical, vertical};
    else return {vertical, horizontal};
}

int playerMove(char moveChar) {
    if (moveChar != 'L' && moveChar != 'R' && moveChar != 'U' && moveChar != 'D') return 0;

    Cell cur = grid[px][py];
    bool followed = (moveChar == cur.dir);
    bool wasCorrect = cur.correct;

    int nx = px, ny = py;
    if (moveChar == 'L') ny--;
    else if (moveChar == 'R') ny++;
    else if (moveChar == 'U') nx--;
    else if (moveChar == 'D') nx++;

    // Check for exit
    if (nx < 0 || nx >= n || ny < 0 || ny >= n) {
        if (moveChar == correctDir && hasKey) {
            return 1;
            
        } // Win
        else return -1; // Game over
    }

    bool goodMove = (wasCorrect && followed) || (!wasCorrect && !followed);
    if (!goodMove) {
        flipKNeighborsToWrong(nx, ny, 1 + rand() % 2);
        powerLeft--;
        if (powerLeft < 0) return -1;
    } else if (rand() % 2) flipOneNeighborToCorrect(nx, ny);

    px = nx; py = ny;
    if (px == keyx && py == keyy) {
        hasKey = true;
        keyx = keyy = -1;
    }

    return 0; // Continue
}


const char* getCurrentDirectionImpl() {
    static std::string dirStr;
    if (grid.empty()) return "";
    dirStr = std::string(1, grid[px][py].dir);
    return dirStr.c_str();
}

const char* getHintImpl() {
    static std::string hintStr;
    if (hintsLeft <= 0) {
        hintStr = "No hints left.";
        return hintStr.c_str();
    }
    hintsLeft--;
    auto h = hintTwoDirectionsToKey();
    if (h.first == 'N' && h.second == 'N') {
        hintStr = "Key already collected.";
    } else if (h.first == h.second) {
        hintStr = std::string("Key is ") + h.first + " from you.";
    } else {
        hintStr = std::string("Key is ") + h.first + " and " + h.second + " from you.";
    }
    return hintStr.c_str();
}
