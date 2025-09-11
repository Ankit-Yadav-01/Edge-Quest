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
    //cout<<correctDir<<endl;
    int total = n*n;
    int numCorrect = total/4;
    vector<int> indices(total);
    for(int i=0;i<total;i++) indices[i]=i;
    random_shuffle(indices.begin(), indices.end());

    for(int k=0;k<numCorrect;k++) {
        int idx = indices[k];
        int i = idx/n, j=idx%n;
        grid[i][j].dir = correctDir;
        grid[i][j].correct = true;
        correctCount++;
    }
    for(int k=numCorrect;k<total;k++) {
        int idx = indices[k];
        int i = idx/n, j=idx%n;
        grid[i][j].dir = randWrongDir();
        grid[i][j].correct = false;
    }

    px = py = n/2;

    vector<int> freeIdx;
    for(int i=0;i<total;i++) {
        int r=i/n, c=i%n;
        if(!(r==px && c==py)) freeIdx.push_back(i);
    }
    int choose = freeIdx[rand()%freeIdx.size()];
    keyx = choose/n;
    keyy = choose%n;
    hasKey = false;
    //cout<<keyx<<" "<<keyy<<endl;
}

vector<pair<int,int> > getNeighbors(int x, int y) {
    vector<pair<int,int> > res;
    if(x>0) res.push_back({x-1,y});
    if(x<n-1) res.push_back({x+1,y});
    if(y>0) res.push_back({x,y-1});
    if(y<n-1) res.push_back({x,y+1});
    return res;
}

bool isVisible(int x, int y) {
    return abs(x-px)<=1 && abs(y-py)<=1;
}

void flipOneNeighborToCorrect(int x, int y) {
    vector<pair<int,int>> neigh = getNeighbors(x,y);
    vector<pair<int,int>> incorrects;
    for(auto p: neigh) if(!grid[p.first][p.second].correct) incorrects.push_back(p);
    if(incorrects.empty()) return;
    int idx = rand()%incorrects.size();
    int nx = incorrects[idx].first, ny = incorrects[idx].second;
    grid[nx][ny].correct = true;
    grid[nx][ny].dir = correctDir;
    correctCount++;
}

void flipKNeighborsToWrong(int x, int y, int k) {
    vector<pair<int,int>> neigh = getNeighbors(x,y);
    if(neigh.empty()) return;
    vector<int> idxs(neigh.size());
    iota(idxs.begin(), idxs.end(),0);
    random_shuffle(idxs.begin(), idxs.end());
    int take = min(k,(int)neigh.size());
    for(int t=0;t<take;t++){
        int iidx = idxs[t];
        int nx=neigh[iidx].first, ny=neigh[iidx].second;
        if(grid[nx][ny].correct){
            grid[nx][ny].correct=false;
            correctCount--;
            grid[nx][ny].dir=randWrongDir();
        } else {
            char old=grid[nx][ny].dir, nw;
            do{ nw=randWrongDir(); } while(nw==old);
            grid[nx][ny].dir = nw;
        }
    }
}

void printFullGrid() {
    cout << "Full Grid View:\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==px && j==py) cout << "P ";
            else if(isVisible(i,j)) cout << grid[i][j].dir << " ";
            else cout << "# ";
        }
        cout << "\n";
    }
    cout << "Your cell direction: " << grid[px][py].dir << "\n";
    if(hasKey) cout << "You have the key! Now find the exit.\n";
    else cout << "You need to find the key first.\n";
    cout << "Player at: (" << px << "," << py << ")\n";
    cout << "Power: " << powerLeft << "   Hints left: " << hintsLeft << "\n";
}

pair<char,char> hintTwoDirectionsToKey() {
    if(keyx==-1 && keyy==-1) return {'N','N'};
    int dx=keyx-px, dy=keyy-py;
    char vertical='N', horizontal='N';
    if(dx<0) vertical='U'; else if(dx>0) vertical='D';
    if(dy<0) horizontal='L'; else if(dy>0) horizontal='R';
    if(dx==0 && dy==0) return {'N','N'};
    else if(dx==0) return {horizontal,horizontal};
    else if(dy==0) return {vertical,vertical};
    else return {vertical,horizontal};
}

bool playerMove(char moveChar) {
    if(moveChar!='L' && moveChar!='R' && moveChar!='U' && moveChar!='D') return true;

    Cell cur = grid[px][py];
    bool followed = (moveChar==cur.dir);
    bool wasCorrect = cur.correct;

    int nx=px, ny=py;
    if(moveChar=='L') ny--; else if(moveChar=='R') ny++;
    else if(moveChar=='U') nx--; else if(moveChar=='D') nx++;

    if(nx<0 || nx>=n || ny<0 || ny>=n){
        if(moveChar==correctDir && hasKey){
            cout << "You moved out through the correct side and you had the key. LEVEL COMPLETED!\n";
            return false;
        } else{
            cout << "You tried to exit incorrectly (or without the key). GAME OVER.\n";
            return false;
        }
    }

    bool goodMove = (wasCorrect && followed) || (!wasCorrect && !followed);
    if(goodMove){
        if(rand()%2) flipOneNeighborToCorrect(nx,ny);
    } else{
        flipKNeighborsToWrong(nx,ny,1+rand()%2);
        powerLeft--;
        if(powerLeft<0){
            cout << "Power dropped below 0. GAME OVER.\n";
            return false;
        }
    }

    px=nx; py=ny;

    if(px==keyx && py==keyy){
        cout << "You found the key!\n";
        hasKey = true;
        keyx = keyy = -1;
    }

    if(correctCount<=0){
        cout << "No correct directions left. GAME OVER.\n";
        return false;
    }
    return true;
}

void printInstructions() {
    cout << "=====================================\n";
    cout << "          MAZE GAME INSTRUCTIONS      \n";
    cout << "=====================================\n";
    cout << "1. You are in a maze of size n x n (odd n). current n = \n"<<n;
    cout << "2. Each cell has a hidden direction (L/R/U/D).\n";
    cout << "3. Only your current cell and its 8 neighbors are visible.\n";
    cout << "4. Player is marked as 'P'.\n";
    cout << "5. Directions of neighbors are displayed, all other cells are hidden (#).\n";
    cout << "6. You can move using U (up), D (down), L (left), R (right).\n";
    cout << "7. Only one global correct direction exists in the maze.\n";
    cout << "8. If you move in the direction shown by the cell and it is correct, \n";
    cout << "   one neighbor of the new cell may flip to correct.\n";
    cout << "9. If you move in the direction shown by the cell and it is wrong, \n";
    cout << "   one neighbor of the new cell may flip to wrong.\n";
    cout << "10. If you move in any other direction, flips happen based on whether \n";
    cout << "    the current cell was correct or wrong.\n";
    cout << "11. You start with a power of 3. Each wrong move decreases power by 1.\n";
    cout << "    If power becomes negative, GAME OVER.\n";
    cout << "12. There is only one exit in the maze, located at the edge in the correct global direction.\n";
    cout << "13. You must find a hidden key before you can exit.\n";
    cout << "14. You have 3 hints. After using a hint, you will know the approximate direction of the key.\n";
    cout << "15. The game ends if:\n";
    cout << "    - You exit through the wrong side or without the key.\n";
    cout << "    - Your power becomes negative.\n";
    cout << "    - There are no correct cells left in the maze.\n";
    cout << "Good luck!\n";
}

int main(){
    srand(time(NULL));
    int level = 0;

    cout << "Have you played this game before? (y/n): ";
    string response;
    getline(cin, response);

    if(!response.empty() && (response[0]=='y' || response[0]=='Y')){
        cout << "What is the maximum level you have reached? ";
        int maxLevel;
        cin >> maxLevel;
        cin.ignore(); // clear newline from buffer

        cout << "Do you want to continue from level " << maxLevel << "? (y/n): ";
        string cont;
        getline(cin, cont);

        if(!cont.empty() && (cont[0]=='y' || cont[0]=='Y')){
            level = maxLevel;
        } else {
            level = 0;
        }
    }

    while(true){
        n = 5 + 2*level;
        powerLeft = (n+1)/2;
        hintsLeft = (n+1)/2;

        cout << "\n=== LEVEL " << level << " === Grid size: " << n << " ===\n";
        initGrid(n);
        printInstructions();
        cout << "Press Enter to start...";
        string dummy;
        getline(cin,dummy);
        clearScreen();
        printFullGrid();

        while(true){
            cout << "Enter command: L/R/U/D to move, H for hint: ";
            string input;
            getline(cin,input);
            if(input.empty()) continue;
            char cmd = toupper(input[0]);

            if(cmd=='H'){
                if(hintsLeft<=0) cout << "No hints left.\n";
                else {
                    pair<char,char> h = hintTwoDirectionsToKey();
                    if(h.first=='N' && h.second=='N') cout << "Key already collected.\n";
                    else if(h.first==h.second) cout << "Hint: key is " << h.first << " from you.\n";
                    else cout << "Hint: key is " << h.first << " and " << h.second << " from you.\n";
                    hintsLeft--;
                }
                continue;
            }

            bool alive = playerMove(cmd);
            if(!alive){
                if(px>=0 && px<n && py>=0 && py<n && hasKey) break; 
                else { cout << "Game over at level " << level << ".\n"; return 0; }
            }

            clearScreen();
            printFullGrid();
        }

        cout << "Level " << level << " completed! Moving to next level...\n";
        level++;
    }

    return 0;
}
