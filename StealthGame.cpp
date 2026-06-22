#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

struct Guard {
    int r, c;
    char dir; // '<', '>', '^', 'v'
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

bool isBlocked(char cell) {
    // Anything that blocks movement
    return (cell == '#' || cell == '<' || cell == '>' ||
            cell == '^' || cell == 'v' || cell == '$' || cell == '@');
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

void updateGuards(vector<string> &grid, vector<Guard> &guards) {
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

        bool blocked = false;
        if (!inBounds(nr, nc, grid)) {
            blocked = true;
        } else {
            char cell = grid[nr][nc];
            if (cell != ' ' && cell != '.') {
                blocked = true;
            }
        }

        if (blocked) {
            // flip direction
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

            if (inBounds(nr, nc, grid)) {
                char cell = grid[nr][nc];
                if (cell == ' ' || cell == '.') {
                    g.r = nr;
                    g.c = nc;
                }
            }
        } else {
            g.r = nr;
            g.c = nc;
        }
    }

    // Re-place guards
    for (const auto &g : guards) {
        if (inBounds(g.r, g.c, grid)) {
            grid[g.r][g.c] = g.dir;
        }
    }
}

bool runLevel(const Level &level) {
    vector<string> base = level.layout;
    vector<string> grid = base;

    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    int playerR = -1, playerC = -1;
    vector<Guard> guards;

    // Parse initial layout
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char ch = grid[r][c];
            if (ch == '@') {
                playerR = r;
                playerC = c;
                grid[r][c] = ' ';
            } else if (ch == '<' || ch == '>' || ch == '^' || ch == 'v') {
                Guard g{r, c, ch};
                guards.push_back(g);
                grid[r][c] = ' ';
            }
        }
    }

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
        cout << "Move (WASD), or Q to quit level: ";

        string input;
        getline(cin, input);
        if (input.empty()) continue;
        char cmd = static_cast<char>(tolower(input[0]));
        if (cmd == 'q') {
            cout << "You quit the level.\n";
            return false;
        }

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
        updateGuards(grid, guards);

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
    Level level1{
        "A New Enemy",
        {
            "####################",
            "#@   #         $   #",
            "#    #   ####      #",
            "#    #             #",
            "#    #######   ^   #",
            "#              #   #",
            "####################"
        }
    };

    Level level2{
        "The Truth of the Weapon",
        {
            "####################",
            "#@   #   v      $  #",
            "#    #   ####      #",
            "#    #        <    #",
            "#    #######       #",
            "#              #   #",
            "####################"
        }
    };

    vector<Level> levels = {level1, level2};

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
