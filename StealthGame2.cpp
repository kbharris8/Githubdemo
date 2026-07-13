#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
using namespace std;

struct Guard {
    int r, c;
    char dir;   // '<', '>', '^', 'v'
    bool patrol; // false = line guard (flip), true = area guard (clockwise)
};

struct Level {
    string name;
    vector<string> layout;
};

string toLower(const string &s) {
    string t = s;
    for (char &ch : t) ch = static_cast<char>(tolower(ch));
    return t;
}

void printLevel(const vector<string> &grid) {
    for (const auto &row : grid) {
        cout << row << '\n';
    }
}

bool inBounds(int r, int c, const vector<string> &grid) {
    return r >= 0 && r < (int)grid.size() && c >= 0 && c < (int)grid[0].size();
}

bool playerSeen(const vector<string> &grid) {
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char ch = grid[r][c];
            if (ch == '<' || ch == '>' || ch == '^' || ch == 'v') {
                int dr = 0, dc = 0;
                if (ch == '<') dc = -1;
                else if (ch == '>') dc = 1;
                else if (ch == '^') dr = -1;
                else if (ch == 'v') dr = 1;

                int rr = r + dr;
                int cc = c + dc;
                while (inBounds(rr, cc, grid)) {
                    char cell = grid[rr][cc];
                    if (cell == '@') {
                        return true; // player in line of sight
                    }
                    if (cell != ' ' && cell != '.') {
                        // vision blocked by any other object
                        break;
                    }
                    rr += dr;
                    cc += dc;
                }
            }
        }
    }
    return false;
}

// Doors: base has uppercase letters 'A','B',... as door groups.
// Switches: base has lowercase letters 'a','b',... as switch groups.
// doorOpen[group] == true => door cells act like empty space in grid.
// doorOpen[group] == false => door cells act like walls ('#') in grid.
void applyDoorStates(vector<string> &grid,
                     const vector<string> &base,
                     const map<char,bool> &doorOpen)
{
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char b = base[r][c];
            if (isupper(b)) {
                auto it = doorOpen.find(b);
                bool open = (it != doorOpen.end() && it->second);
                grid[r][c] = open ? ' ' : '#';
            }
        }
    }
}

void toggleDoors(char group,
                 vector<string> &grid,
                 const vector<string> &base,
                 map<char,bool> &doorOpen)
{
    group = toupper(group);
    doorOpen[group] = !doorOpen[group];
    applyDoorStates(grid, base, doorOpen);
}

void updateGuards(vector<string> &grid,
                  vector<Guard> &guards,
                  const vector<string> &base,
                  map<char,bool> &doorOpen)
{
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    // Clear guard chars from grid; we'll re-place them after movement
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '<' || grid[r][c] == '>' ||
                grid[r][c] == '^' || grid[r][c] == 'v') {
                grid[r][c] = ' ';
            }
        }
    }

    // Move each guard
    for (auto &g : guards) {
        int dr = 0, dc = 0;
        if (g.dir == '<') dc = -1;
        else if (g.dir == '>') dc = 1;
        else if (g.dir == '^') dr = -1;
        else if (g.dir == 'v') dr = 1;

        int nr = g.r + dr;
        int nc = g.c + dc;

        auto cellIsFree = [&](int rr, int cc) {
            if (!inBounds(rr, cc, grid)) return false;
            char cell = grid[rr][cc];
            // free if empty or floor
            return (cell == ' ' || cell == '.');
        };

        bool blocked = !cellIsFree(nr, nc);

        if (!g.patrol) {
            // Original line guard: flip direction when blocked
            if (blocked) {
                if (g.dir == '<') g.dir = '>';
                else if (g.dir == '>') g.dir = '<';
                else if (g.dir == '^') g.dir = 'v';
                else if (g.dir == 'v') g.dir = '^';

                dr = dc = 0;
                if (g.dir == '<') dc = -1;
                else if (g.dir == '>') dc = 1;
                else if (g.dir == '^') dr = -1;
                else if (g.dir == 'v') dr = 1;

                nr = g.r + dr;
                nc = g.c + dc;

                if (cellIsFree(nr, nc)) {
                    g.r = nr;
                    g.c = nc;
                }
            } else {
                g.r = nr;
                g.c = nc;
            }
        } else {
            // Patrolling guard: clockwise turn when blocked
            if (blocked) {
                // clockwise: ^ -> > -> v -> < -> ^
                if (g.dir == '^') g.dir = '>';
                else if (g.dir == '>') g.dir = 'v';
                else if (g.dir == 'v') g.dir = '<';
                else if (g.dir == '<') g.dir = '^';

                dr = dc = 0;
                if (g.dir == '<') dc = -1;
                else if (g.dir == '>') dc = 1;
                else if (g.dir == '^') dr = -1;
                else if (g.dir == 'v') dr = 1;

                nr = g.r + dr;
                nc = g.c + dc;

                if (cellIsFree(nr, nc)) {
                    g.r = nr;
                    g.c = nc;
                }
            } else {
                g.r = nr;
                g.c = nc;
            }
        }

        // If guard steps on a switch, toggle doors
        if (inBounds(g.r, g.c, base)) {
            char b = base[g.r][g.c];
            if (islower(b)) {
                toggleDoors(b, grid, base, doorOpen);
            }
        }
    }

    // Re-place guards
    for (const auto &g : guards) {
        if (inBounds(g.r, g.c, grid)) {
            grid[g.r][g.c] = g.dir;
        }
    }
}

void inspectTile(int rr, int cc,
                 const vector<string> &base,
                 const vector<string> &grid,
                 const vector<Guard> &guards,
                 int playerR, int playerC,
                 const map<char,bool> &doorOpen)
{
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();
    if (!inBounds(rr, cc, grid)) {
        cout << "Tile (" << rr << "," << cc << ") is out of bounds.\n";
        return;
    }

    // Player?
    if (rr == playerR && cc == playerC) {
        cout << "Tile (" << rr << "," << cc << "): You (the player).\n";
        return;
    }

    // Guard?
    for (const auto &g : guards) {
        if (g.r == rr && g.c == cc) {
            cout << "Tile (" << rr << "," << cc << "): Guard facing '" << g.dir
                 << "', movement type: "
                 << (g.patrol ? "patrolling (clockwise turns)" : "line (flip on block)")
                 << ".\n";
            return;
        }
    }

    char b = base[rr][cc];
    char gcell = grid[rr][cc];

    // Doors / switches by base
    if (isupper(b)) {
        auto it = doorOpen.find(b);
        bool open = (it != doorOpen.end() && it->second);
        cout << "Tile (" << rr << "," << cc << "): Door, group '" << b
             << "', currently " << (open ? "OPEN" : "CLOSED") << ".\n";
        return;
    }
    if (islower(b)) {
        char group = toupper(b);
        auto it = doorOpen.find(group);
        bool open = (it != doorOpen.end() && it->second);
        cout << "Tile (" << rr << "," << cc << "): Switch, group '" << group
             << "'. Doors in this group are currently "
             << (open ? "OPEN" : "CLOSED") << ".\n";
        return;
    }

    // Other objects by grid
    if (gcell == '#') {
        cout << "Tile (" << rr << "," << cc << "): Wall.\n";
    } else if (gcell == '$') {
        cout << "Tile (" << rr << "," << cc << "): Goal.\n";
    } else if (gcell == '.' ) {
        cout << "Tile (" << rr << "," << cc << "): Floor.\n";
    } else if (gcell == ' ' ) {
        cout << "Tile (" << rr << "," << cc << "): Empty space.\n";
    } else {
        cout << "Tile (" << rr << "," << cc << "): '" << gcell << "'.\n";
    }
}

bool runLevel(const Level &level) {
    vector<string> base = level.layout;
    vector<string> grid = base;

    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    int playerR = -1, playerC = -1;
    vector<Guard> guards;
    map<char,bool> doorOpen; // default false (closed)

    // Parse initial layout
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char ch = grid[r][c];
            if (ch == '@') {
                playerR = r;
                playerC = c;
                grid[r][c] = ' ';
                base[r][c] = ' '; // player not part of base
            } else if (ch == '<' || ch == '>' || ch == '^' || ch == 'v') {
                Guard g{r, c, ch, false};
                guards.push_back(g);
                grid[r][c] = ' ';
                base[r][c] = ' '; // guard not part of base
            } else if (ch == 'l' || ch == 'r' || ch == 'u' || ch == 'd') {
                // Patrolling guards encoded as l/r/u/d in layout
                char dir = '<';
                if (ch == 'l') dir = '<';
                else if (ch == 'r') dir = '>';
                else if (ch == 'u') dir = '^';
                else if (ch == 'd') dir = 'v';
                Guard g{r, c, dir, true};
                guards.push_back(g);
                grid[r][c] = ' ';
                base[r][c] = ' '; // guard not part of base
            } else if (isupper(ch)) {
                // Door group; start closed by default
                doorOpen[ch] = false;
            } else if (islower(ch)) {
                // Switch group; door state will be set when toggled
                // nothing to change in grid; switches don't block movement
                grid[r][c] = ' ';
            }
        }
    }

    // Apply initial door states (all closed)
    applyDoorStates(grid, base, doorOpen);

    bool running = true;
    while (running) {
        // Rebuild display grid from base + dynamic entities
        vector<string> display = grid;
        display[playerR][playerC] = '@';
        for (const auto &g : guards) {
            display[g.r][g.c] = g.dir;
        }

        cout << "\nLevel: " << level.name << "\n";
        printLevel(display);
        cout << "Move (WASD), type 'inspect', or Q to quit level: ";

        string input;
        getline(cin, input);
        if (input.empty()) continue;
        string lowInput = toLower(input);

        if (lowInput == "q" || lowInput == "quit") {
            cout << "You quit the level.\n";
            return false;
        }

        if (lowInput == "inspect") {
            cout << "Enter row (0-indexed): ";
            string rs, cs;
            getline(cin, rs);
            cout << "Enter column (0-indexed): ";
            getline(cin, cs);
            int rr = -1, cc = -1;
            try {
                rr = stoi(rs);
                cc = stoi(cs);
            } catch (...) {
                cout << "Invalid row/column input.\n";
                continue;
            }
            inspectTile(rr, cc, base, grid, guards, playerR, playerC, doorOpen);
            // Inspect does NOT cost a turn: skip guard movement and sight checks
            continue;
        }

        char cmd = static_cast<char>(tolower(input[0]));
        int dr = 0, dc = 0;
        if (cmd == 'w') dr = -1;
        else if (cmd == 's') dr = 1;
        else if (cmd == 'a') dc = -1;
        else if (cmd == 'd') dc = 1;
        else {
            cout << "Invalid input.\n";
            continue;
        }

        int nr = playerR + dr;
        int nc = playerC + dc;

        if (!inBounds(nr, nc, grid)) {
            cout << "You bump into the edge.\n";
            continue;
        }

        // Build a temp display to check collisions with guards/goal
        vector<string> temp = grid;
        for (const auto &g : guards) {
            temp[g.r][g.c] = g.dir;
        }

        char dest = temp[nr][nc];

        if (dest == '#') {
            cout << "You bump into a wall.\n";
            continue; // no turn spent
        }
        if (dest == '<' || dest == '>' || dest == '^' || dest == 'v') {
            cout << "You ran into a guard. You lose.\n";
            return false;
        }
        if (dest == '$') {
            cout << "You reached the goal! You win!\n";
            return true;
        }

        // Move player
        playerR = nr;
        playerC = nc;

        // If player steps on a switch, toggle doors
        if (inBounds(playerR, playerC, base)) {
            char b = base[playerR][playerC];
            if (islower(b)) {
                toggleDoors(b, grid, base, doorOpen);
            }
        }

        // After player moves, check if seen
        temp = grid;
        temp[playerR][playerC] = '@';
        for (const auto &g : guards) {
            temp[g.r][g.c] = g.dir;
        }
        if (playerSeen(temp)) {
            cout << "A guard spotted you! You lose.\n";
            return false;
        }

        // Move guards
        updateGuards(grid, guards, base, doorOpen);

        // Rebuild temp and check sight again
        temp = grid;
        temp[playerR][playerC] = '@';
        for (const auto &g : guards) {
            temp[g.r][g.c] = g.dir;
        }
        if (playerSeen(temp)) {
            cout << "A guard spotted you after moving! You lose.\n";
            return false;
        }
    }

    return false;
}

int main() {
    // Level 1: multiple groups of switches/doors
    Level level1{
        "Multi Door Groups",
        {
            "####################",
            "#@   a   A     $   #",
            "#    #   ####      #",
            "#    #   b   B     #",
            "#    #######       #",
            "#              #   #",
            "####################"
        }
    };

    // Level 2: patrolling guard moves in a rectangle (encoded as l/r/u/d)
    Level level2{
        "Rectangle Patrol",
        {
            "####################",
            "#@       ########  #",
            "#        #      #  #",
            "#   l    #      #  #",
            "#        ########  #",
            "#              $   #",
            "####################"
        }
    };

    // Level 3: patrolling guard moves in a more complex pattern
    Level level3{
        "Complex Patrol",
        {
            "####################",
            "#@   ####      $   #",
            "#    #  r         ##",
            "#    #  ######    ##",
            "#    #            ##",
            "#              #   #",
            "####################"
        }
    };

    // Level 4: guard walks over a switch
    Level level4{
        "Guard Uses Switch",
        {
            "####################",
            "#@   #   v      $  #",
            "#    #   ####      #",
            "#    #   a   A    ##",
            "#    #######       #",
            "#              #   #",
            "####################"
        }
    };

    vector<Level> levels = {level1, level2, level3, level4};

    while (true) {
        cout << "Welcome to Ultra-Spy!\n";
        cout << "Select a level:\n";
        for (size_t i = 0; i < levels.size(); ++i) {
            cout << (i + 1) << ") " << levels[i].name << "\n";
        }
        cout << "Or type Q to quit.\n";
        cout << "Your choice: ";

        string choice;
        getline(cin, choice);
        if (choice.empty()) continue;

        string low = toLower(choice);
        if (low == "q" || low == "quit") {
            cout << "Goodbye, agent.\n";
            break;
        }

        int index = -1;

        if (isdigit(choice[0])) {
            int num = choice[0] - '0';
            if (num >= 1 && num <= (int)levels.size()) {
                index = num - 1;
            }
        } else {
            for (size_t i = 0; i < levels.size(); ++i) {
                if (toLower(levels[i].name) == low) {
                    index = (int)i;
                    break;
                }
            }
        }

        if (index == -1) {
            cout << "Invalid selection.\n\n";
            continue;
        }

        bool won = runLevel(levels[index]);
        cout << (won ? "Mission success.\n\n" : "Mission failed.\n\n");
    }

    return 0;
}
