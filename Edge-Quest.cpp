#include <bits/stdc++.h>
using namespace std;

struct Cell {
    char dir;     
    bool correct; 
};

int n;
vector<vector<Cell> > grid;
int correctCount = 0;
int px, py;            
char correctDir;       
int powerLeft;     
int hintsLeft;     
int keyx = -1, keyy = -1;
bool hasKey = false;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

char randWrongDir() {
    string d = "LRUD";
    char c;
    do { c = d[rand()%4]; } while(c == correctDir);
    return c;
}

void initGrid(int n) {
    grid.assign(n, vector<Cell>(n));
    correctCount = 0;

    string d = "LRUD";
    correctDir = d[rand() % 4];
    int total = n * n;
    int numCorrect = total / 4;
    vector<int> indices(total);
    for (int i = 0; i < total; i++) indices[i] = i;
    random_shuffle(indices.begin(), indices.end());


    for (int k = 0; k < numCorrect; k++) {
        int idx = indices[k];
        int i = idx / n;
        int j = idx % n;
        grid[i][j].dir = correctDir;
        grid[i][j].correct = true;
        correctCount++;
    }

    for (int k = numCorrect; k < total; k++) {
        int idx = indices[k];
        int i = idx / n;
        int j = idx % n;
        grid[i][j].dir = randWrongDir();
        grid[i][j].correct = false;
    }

    px = py = n / 2;

    vector<int> freeIdx;
    for (int i = 0; i < total; i++) {
        int r = i / n, c = i % n;
        if (!(r == px && c == py)) freeIdx.push_back(i);
    }
    int choose = freeIdx[rand() % freeIdx.size()];
    keyx = choose / n;
    keyy = choose % n;
    hasKey = false;
    //cout<<keyx<<" "<<keyy<<endl;
}

vector<pair<int,int> > getNeighbors(int x, int y) {
    vector<pair<int,int> > res;
    if (x > 0) res.push_back(make_pair(x-1, y));
    if (x < n-1) res.push_back(make_pair(x+1, y));
    if (y > 0) res.push_back(make_pair(x, y-1));
    if (y < n-1) res.push_back(make_pair(x, y+1));
    return res;
}

bool isVisible(int x, int y) {
    return abs(x - px) <= 1 && abs(y - py) <= 1;
}

void flipOneNeighborToCorrect(int x, int y) {
    vector<pair<int,int> > neigh = getNeighbors(x, y);
    vector<pair<int,int> > incorrects;
    for (int i = 0; i < (int)neigh.size(); i++) {
        int nx = neigh[i].first, ny = neigh[i].second;
        if (!grid[nx][ny].correct) incorrects.push_back(make_pair(nx, ny));
    }
    if (incorrects.empty()) return;
    int idx = rand() % incorrects.size();
    int nx = incorrects[idx].first, ny = incorrects[idx].second;
    grid[nx][ny].correct = true;
    grid[nx][ny].dir = correctDir;
    correctCount++;
}

void flipKNeighborsToWrong(int x, int y, int k) {
    vector<pair<int,int> > neigh = getNeighbors(x, y);
    if (neigh.empty()) return;
    vector<int> idxs;
    for (int i = 0; i < (int)neigh.size(); i++) idxs.push_back(i);
    random_shuffle(idxs.begin(), idxs.end());
    int take = k;
    if (take > (int)neigh.size()) take = neigh.size();

    for (int t = 0; t < take; t++) {
        int iidx = idxs[t];
        int nx = neigh[iidx].first, ny = neigh[iidx].second;
        if (grid[nx][ny].correct) {
            grid[nx][ny].correct = false;
            correctCount--;
            grid[nx][ny].dir = randWrongDir();
        } else {
            char old = grid[nx][ny].dir;
            char nw;
            do {
                nw = randWrongDir();
            } while (nw == old);
            grid[nx][ny].dir = nw;
        }
    }
}

void printFullGrid() {
    cout << "Full Grid View:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == px && j == py) {
                cout << "P ";
            } else if (isVisible(i, j)) {
                cout << grid[i][j].dir << " ";
            } else {
                cout << "# ";
            }
        }
        cout << "\n";
    }
    cout << "Your cell direction: " << grid[px][py].dir << "\n";
    if (px == keyx && py == keyy) {
        cout << "You found the key here!\n";
        hasKey = true;
    }
    cout << "Player at: (" << px << "," << py << ")\n";
    cout << "Power: " << powerLeft << "   Hints left: " << hintsLeft << "\n";
}

pair<char, char> hintTwoDirectionsToKey() {
    if (keyx == -1 && keyy == -1) {
        return make_pair('N', 'N');
    }
    
    int dx = keyx - px;
    int dy = keyy - py; 
    char vertical = 'N', horizontal = 'N';
    
    if (dx < 0) vertical = 'U';      
    else if (dx > 0) vertical = 'D';   
    if (dy < 0) horizontal = 'L'; 
    else if (dy > 0) horizontal = 'R';    
    if (dx == 0 && dy == 0) {
        return make_pair('N', 'N');
    } else if (dx == 0) {
        return make_pair(horizontal, horizontal);
    } else if (dy == 0) {
        return make_pair(vertical, vertical);
    } else {
        return make_pair(vertical, horizontal);
    }
}

bool playerMove(char moveChar) {
    if (moveChar != 'L' && moveChar != 'R' && moveChar != 'U' && moveChar != 'D') return true;
    Cell cur = grid[px][py];
    bool followed = (moveChar == cur.dir);
    bool wasCorrect = cur.correct;
    int nx = px, ny = py;
    if (moveChar == 'L') ny--;
    else if (moveChar == 'R') ny++;
    else if (moveChar == 'U') nx--;
    else if (moveChar == 'D') nx++;
    if (nx < 0 || nx >= n || ny < 0 || ny >= n) {
        if (moveChar == correctDir && hasKey) {
            cout << "You moved out through the correct side and you had the key. YOU WIN!\n";
            return false;
        } else {
            cout << "You tried to exit incorrectly (or without the key). GAME OVER.\n";
            return false;
        }
    }
    bool goodMove = (wasCorrect && followed) || (!wasCorrect && !followed);
    if (goodMove) {
        int flip = rand() % 2; 
        if (flip == 1) flipOneNeighborToCorrect(nx, ny);
    } else {
        int howMany = 1 + (rand() % 2); // 1 or 2
        flipKNeighborsToWrong(nx, ny, howMany);
        powerLeft--;
        if (powerLeft < 0) {
            cout << "Power dropped below 0. GAME OVER.\n";
            return false;
        }
    }
    px = nx; py = ny;
    if (px == keyx && py == keyy) {
        cout << "You found the key!\n";
        hasKey = true;
        keyx = keyy = -1;
    }
    if (correctCount <= 0) {
        cout << "No correct directions left. GAME OVER.\n";
        return false;
    }
    return true;
}

int main() {
    srand((unsigned int)time(NULL));
    cout << "Enter odd grid size n: ";
    cin >> n;
    if (n % 2 == 0 || n <= 1) {
        cout << "n must be an odd integer > 1.\n";
        return 0;
    }

    initGrid(n);

    cout << "Game start. Full grid will be shown with limited visibility.\n";
    cout << "Starting power: " << powerLeft << "\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    clearScreen();
    printFullGrid();

    while (true) {
        cout << "Enter command: L/R/U/D to move, H for a hint (uses one hint). Input: ";
        char cmd;
        cin >> cmd;
        if (cmd >= 'a' && cmd <= 'z') cmd = cmd - 'a' + 'A';

        if (cmd == 'H') {
            if (hintsLeft <= 0) {
                cout << "No hints left.\n";
            } else {
                pair<char, char> hints = hintTwoDirectionsToKey();
                if (hints.first == 'N' && hints.second == 'N') {
                    cout << "Key already collected.\n";
                } else {
                    cout << "Hint: key is ";
                    if (hints.first == hints.second && hints.first != 'N') {
                        cout << hints.first << " from you.\n";
                    } else if (hints.first != 'N' && hints.second != 'N') {
                        cout << hints.first << " and " << hints.second << " from you.\n";
                    } else {
                        char validDir = (hints.first != 'N') ? hints.first : hints.second;
                        cout << validDir << " from you.\n";
                    }
                }
                hintsLeft--;
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            clearScreen();
            printFullGrid();
            continue;
        }
        bool alive = playerMove(cmd);
        if (!alive) break;
        clearScreen();
        printFullGrid();
    }

    cout << "Thanks for playing.\n";
    return 0;
} 