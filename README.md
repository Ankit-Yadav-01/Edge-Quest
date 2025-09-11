Edge-Quest Game (C++)

A console-based, memory-driven Edge finding game implemented in C++, where the player navigates a hidden maze, collects a key, and finds the exit while managing limited power and remembering cell directions.


---

Features

    1. Dynamic visibility: Only the player’s current cell and its 8 neighbors are visible; all other cells are hidden.
    2. Memory challenge: No markers for correct cells; players must remember directions from previous moves.
    3. Hidden key mechanic: A key is randomly placed in the maze; the player must find it before exiting.
    4. Single exit: Only one correct exit exists at the edge of the maze.  
    5. Power system: The player starts with a limited number of "power" points. Wrong moves decrease power; reaching  zero power triggers warnings, and further wrong moves end the game.
    6. Neighbor flipping: Moving in correct or wrong directions can randomly flip neighbor cells to correct or incorrect, adding unpredictability.
    7. Hints: Player can request up to 3 hints to get the approximate direction of the key.



---
How to Play
    1. Run the program in a terminal.
    2. Enter an odd-sized grid (n x n).
    3. The maze is displayed with:
        P for the player.
        Directions (L, R, U, D) for the 8 neighboring cells.
        # for hidden cells.
    4. Controls:
        U → move up
        L → move left
        D → move down
        R → move right
        H → use a hint to locate the key
    
    5. Rules:

        Only one cell direction is globally correct.
        Following the correct direction may flip a neighbor cell to correct.
        Following a wrong direction may flip neighbor cells to wrong.
        Power decreases on wrong moves. Game over if power < 0.
        Player must collect the hidden key before attempting to exit.

    6. Game ends when:
        Player exits correctly with the key.
        Power drops below zero.
        Player tries to exit incorrectly.
        No correct cells remain in the maze.





---

Requirements
C++11 or above
Terminal/Command Prompt to run the console application

---

How to Run

    1. Clone the repository:
        git clone https://github.com/Ankit-Yadav-01/Edge-Quest.git

    2. Navigate to the project folder:
        cd Edge-Quest

    3. Compile the program:
        g++ Edge-Quest.cpp -o Edge-Quest

    4. Run the game:
        ./Edge-quest






---

Notes

The game uses random initialization, so each run provides a new challenge.
Designed for memory-based gameplay; the player must remember previous cell directions to succeed.
Makes extensive use of ANSI escape codes to clear the screen dynamically. May require compatible terminal (Linux, macOS, Windows PowerShell/Command Prompt).



---
