#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <fstream>
using namespace std;

struct Guard {
    int r, c;
    char dir;   
    bool patrol; 
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
                        return true;
                    }
                    if (cell != ' ' && cell != '.') {
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
                grid[r][c] = open ? ' ' : '+';
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

    cout << "\nSwitch " << group << " activated. Doors in group " << group
         << " are now " << (doorOpen[group] ? "OPEN.\n" : "CLOSED.\n");
}

void updateGuards(vector<string> &grid,
                  vector<Guard> &guards,
                  const vector<string> &base,
                  map<char,bool> &doorOpen,
                  int playerR,
                  int playerC,
                  bool &playerDead)
{
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (grid[r][c] == '<' || grid[r][c] == '>' ||
                grid[r][c] == '^' || grid[r][c] == 'v')
                grid[r][c] = ' ';

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
            return (cell == ' ' || cell == '.' || cell == '@');
        };

        bool blocked = !cellIsFree(nr, nc);

        if (!g.patrol) {
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
            if (blocked) {
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

        if (g.r == playerR && g.c == playerC)
            playerDead = true;

        if (inBounds(g.r, g.c, base)) {
            char b = base[g.r][g.c];
            if (islower(b))
                toggleDoors(b, grid, base, doorOpen);
        }
    }

    for (const auto &g : guards)
        if (inBounds(g.r, g.c, grid))
            grid[g.r][g.c] = g.dir;
}

void inspectTile(int rr, int cc,
                 const vector<string> &base,
                 const vector<string> &grid,
                 const vector<Guard> &guards,
                 int playerR, int playerC,
                 const map<char,bool> &doorOpen)
{
    if (!inBounds(rr, cc, grid)) {
        cout << "Tile (" << rr << "," << cc << ") is out of bounds.\n";
        return;
    }

    if (rr == playerR && cc == playerC) {
        cout << "Tile (" << rr << "," << cc << "): You.\n";
        return;
    }

    for (const auto &g : guards)
        if (g.r == rr && g.c == cc) {
            cout << "Tile (" << rr << "," << cc << "): Guard facing '" << g.dir
                 << "', type: " << (g.patrol ? "patrolling" : "line") << ".\n";
            return;
        }

    char b = base[rr][cc];
    char gcell = grid[rr][cc];

    if (isupper(b)) {
        auto it = doorOpen.find(b);
        bool open = (it != doorOpen.end() && it->second);
        cout << "Tile (" << rr << "," << cc << "): Door " << b
             << " (" << (open ? "OPEN" : "CLOSED") << ").\n";
        return;
    }
    if (islower(b)) {
        char group = toupper(b);
        auto it = doorOpen.find(group);
        bool open = (it != doorOpen.end() && it->second);
        cout << "Tile (" << rr << "," << cc << "): Switch " << group
             << " (doors " << (open ? "OPEN" : "CLOSED") << ").\n";
        return;
    }

    if (gcell == '#') cout << "Tile: Wall.\n";
    else if (gcell == '+') cout << "Tile: Closed door.\n";
    else if (gcell == '$') cout << "Tile: Goal.\n";
    else if (gcell == '.') cout << "Tile: Floor.\n";
    else if (gcell == ' ') cout << "Tile: Empty.\n";
    else cout << "Tile: '" << gcell << "'.\n";
}

bool runLevel(const Level &level) {
    vector<string> base = level.layout;
    vector<string> grid = base;

    int rows = grid.size();
    int cols = grid[0].size();

    int playerR = -1, playerC = -1;
    vector<Guard> guards;
    map<char,bool> doorOpen;

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            char ch = grid[r][c];
            if (ch == '@') {
                playerR = r;
                playerC = c;
                grid[r][c] = ' ';
                base[r][c] = ' ';
            } else if (ch == '<' || ch == '>' || ch == '^' || ch == 'v') {
                guards.push_back({r, c, ch, false});
                grid[r][c] = ' ';
                base[r][c] = ' ';
            } else if (ch == 'l' || ch == 'r' || ch == 'u' || ch == 'd') {
                char dir = (ch=='l'?'<':ch=='r'?'>':ch=='u'?'^':'v');
                guards.push_back({r, c, dir, true});
                grid[r][c] = ' ';
                base[r][c] = ' ';
            } else if (isupper(ch)) {
                doorOpen[ch] = false;
            } else if (islower(ch)) {
                grid[r][c] = ' ';
            }
        }
    
    applyDoorStates(grid, base, doorOpen);

    while (true) {
        vector<string> display = grid;
        display[playerR][playerC] = '@';
        for (const auto &g : guards)
            display[g.r][g.c] = g.dir;

        cout << "\nLevel: " << level.name << "\n";
        printLevel(display);
        cout << "Move (WASD), 'inspect', or Q: ";

        string input;
        getline(cin, input);
        if (input.empty()) continue;

        string low = toLower(input);
        if (low == "q" || low == "quit") {
            cout << "You quit.\n";
            return false;
        }

        if (low == "inspect") {
            cout << "Row: ";
            string rs, cs;
            getline(cin, rs);
            cout << "Col: ";
            getline(cin, cs);
            try {
                inspectTile(stoi(rs), stoi(cs), base, grid, guards, playerR, playerC, doorOpen);
            } catch (...) {
                cout << "Invalid input.\n";
            }
            continue;
        }

        char cmd = tolower(input[0]);
        int dr = 0, dc = 0;
        if (cmd=='w') dr=-1;
        else if (cmd=='s') dr=1;
        else if (cmd=='a') dc=-1;
        else if (cmd=='d') dc=1;
        else {
            cout << "Invalid.\n";
            continue;
        }

        int nr = playerR + dr;
        int nc = playerC + dc;

        if (!inBounds(nr, nc, grid)) {
            cout << "Edge.\n";
            continue;
        }

        vector<string> temp = grid;
        for (const auto &g : guards)
            temp[g.r][g.c] = g.dir;

        char dest = temp[nr][nc];

        if (dest=='#') { cout<<"Wall.\n"; continue; }
        if (dest=='+') { cout<<"Closed door.\n"; continue; }
        if (dest=='<'||dest=='>'||dest=='^'||dest=='v') {
            cout<<"Guard hit you.\n";
            return false;
        }
        if (dest=='$') {
            cout<<"You win.\n";
            return true;
        }

        playerR = nr;
        playerC = nc;

        if (islower(base[playerR][playerC]))
            toggleDoors(base[playerR][playerC], grid, base, doorOpen);

        temp = grid;
        temp[playerR][playerC] = '@';
        for (const auto &g : guards)
            temp[g.r][g.c] = g.dir;

        if (playerSeen(temp)) {
            cout<<"Spotted.\n";
            return false;
        }

        bool dead=false;
        updateGuards(grid, guards, base, doorOpen, playerR, playerC, dead);

        temp = grid;
        temp[playerR][playerC] = '@';
        for (const auto &g : guards)
            temp[g.r][g.c] = g.dir;

        if (dead) {
            cout<<"Guard ran into you.\n";
            return false;
        }
        if (playerSeen(temp)) {
            cout<<"Spotted.\n";
            return false;
        }
    }
}

// ---------- NEW: EDITOR / SAVE / LOAD ----------

void printNumberedGrid(const vector<string> &grid) {
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    cout << "    ";
    for (int c = 0; c < cols; ++c) cout << (c % 10);
    cout << "\n";

    for (int r = 0; r < rows; ++r) {
        cout << (r % 10) << " | ";
        cout << grid[r] << "\n";
    }
}

int readInt(const string &prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        try {
            int v = stoi(s);
            if (v < minVal || v > maxVal) {
                cout << "Value must be between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            return v;
        } catch (...) {
            cout << "Invalid integer.\n";
        }
    }
}

char readCharFromSet(const string &prompt, const string &allowed) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        if (s.size() != 1) {
            cout << "Enter a single character.\n";
            continue;
        }
        char ch = s[0];
        if (allowed.find(ch) == string::npos) {
            cout << "Invalid character.\n";
            continue;
        }
        return ch;
    }
}

Level createLevelEditor() {
    cout << "Enter level name: ";
    string name;
    getline(cin, name);
    if (name.empty()) name = "Untitled";

    int rows = readInt("Enter number of rows (1–50): ", 1, 50);
    int cols = readInt("Enter number of columns (1–50): ", 1, 50);

    vector<string> layout(rows, string(cols, ' '));

    while (true) {
        cout << "\nLevel Editor - " << name << "\n";
        printNumberedGrid(layout);
        cout << "\nObjects:\n";
        cout << "1) Wall (#)\n";
        cout << "2) Empty ( )\n";
        cout << "3) Floor (.)\n";
        cout << "4) Player (@)\n";
        cout << "5) Goal ($)\n";
        cout << "6) Guard (normal)\n";
        cout << "7) Guard (patrolling)\n";
        cout << "8) Door (A-Z)\n";
        cout << "9) Switch (a-z)\n";
        cout << "10) Inspect tile\n";
        cout << "11) Save and quit\n";

        int choice = readInt("Select object number: ", 1, 11);

        if (choice == 11) {
            Level lvl;
            lvl.name = name;
            lvl.layout = layout;
            return lvl;
        }

        int r = readInt("Row: ", 0, rows - 1);
        int c = readInt("Col: ", 0, cols - 1);

        if (choice == 10) {
            cout << "Tile (" << r << "," << c << "): '" << layout[r][c] << "'\n";
            continue;
        }

        switch (choice) {
            case 1: layout[r][c] = '#'; break;
            case 2: layout[r][c] = ' '; break;
            case 3: layout[r][c] = '.'; break;
            case 4: layout[r][c] = '@'; break;
            case 5: layout[r][c] = '$'; break;
            case 6: {
                char dir = readCharFromSet("Direction (< > ^ v): ", "<>^v");
                layout[r][c] = dir;
                break;
            }
            case 7: {
                char dir = readCharFromSet("Direction (< > ^ v): ", "<>^v");
                char patChar = 'l';
                if (dir == '<') patChar = 'l';
                else if (dir == '>') patChar = 'r';
                else if (dir == '^') patChar = 'u';
                else if (dir == 'v') patChar = 'd';
                layout[r][c] = patChar;
                break;
            }
            case 8: {
                char group = readCharFromSet("Door group (A-Z): ", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                layout[r][c] = group;
                break;
            }
            case 9: {
                char group = readCharFromSet("Switch group (A-Z): ", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                layout[r][c] = (char)tolower(group);
                break;
            }
        }
    }
}

void saveLevelToFile(const Level &lvl) {
    string fname = lvl.name;
    if (fname.size() < 4 || fname.substr(fname.size()-4) != ".lvl")
        fname += ".lvl";

    ofstream out(fname);
    if (!out) {
        cout << "Failed to open file for writing.\n";
        return;
    }

    int rows = (int)lvl.layout.size();
    int cols = (int)lvl.layout[0].size();

    out << lvl.name << "\n";
    out << rows << " " << cols << "\n";
    for (const auto &row : lvl.layout)
        out << row << "\n";

    cout << "Level saved to " << fname << "\n";
}

bool loadLevelFromFile(const string &inputName, Level &lvl) {
    string fname = inputName;
    if (fname.size() < 4 || fname.substr(fname.size()-4) != ".lvl")
        fname += ".lvl";

    ifstream in(fname);
    if (!in) {
        cout << "Failed to open " << fname << "\n";
        return false;
    }

    string name;
    if (!getline(in, name)) {
        cout << "Invalid file format.\n";
        return false;
    }

    int rows, cols;
    if (!(in >> rows >> cols)) {
        cout << "Invalid size line.\n";
        return false;
    }
    string dummy;
    getline(in, dummy);

    vector<string> layout;
    for (int i = 0; i < rows; ++i) {
        string row;
        if (!getline(in, row)) {
            cout << "Not enough rows in file.\n";
            return false;
        }
        if ((int)row.size() != cols) {
            cout << "Row " << i << " has wrong length.\n";
            return false;
        }
        layout.push_back(row);
    }

    lvl.name = name;
    lvl.layout = layout;
    cout << "Loaded level '" << lvl.name << "' from " << fname << "\n";
    return true;
}

// ---------- MAIN WITH ORIGINAL LEVELS + NEW OPTIONS ----------

int main() {
    Level level1{
        "Shifty Situation",
        {
            "####################",
            "#@  < a   A     $  #",
            "#    #   ####      #",
            "#    #   b   B     #",
            "#    #######       #",
            "#              #   #",
            "####################"
        }
    };

    Level level2{
        "Juke the guard",
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

    Level level4{
        "Evade The Guard",
        {
            "####################",
            "#@       v      $  #",
            "#        ####      #",
            "#        a   A    ##",
            "#    #######       #",
            "#              #   #",
            "####################"
        }
    };

    vector<Level> levels = {level1, level2, level3, level4};

    while (true) {
        cout << "Welcome to Ultra-Spy!\n";
        cout << "Select a level:\n";
        for (size_t i = 0; i < levels.size(); ++i)
            cout << (i+1) << ") " << levels[i].name << "\n";
        cout << "C) Create new level\n";
        cout << "L) Load level from file\n";
        cout << "Q) Quit.\n";

        string choice;
        getline(cin, choice);
        if (choice.empty()) continue;

        string low = toLower(choice);
        if (low=="q"||low=="quit") {
            cout<<"Goodbye.\n";
            break;
        }

        if (low=="c") {
            Level newLevel = createLevelEditor();
            saveLevelToFile(newLevel);
            levels.push_back(newLevel);
            cout << "Level added to list.\n\n";
            continue;
        }

        if (low=="l") {
            cout << "Enter level name (with or without .lvl): ";
            string fname;
            getline(cin, fname);
            Level loaded;
            if (loadLevelFromFile(fname, loaded)) {
                levels.push_back(loaded);
                cout << "Level added to list.\n\n";
            } else {
                cout << "Failed to load level.\n\n";
            }
            continue;
        }

        int index=-1;
        if (isdigit(choice[0])) {
            int num = choice[0]-'0';
            if (num>=1 && num<=(int)levels.size())
                index = num-1;
        }

        if (index==-1) {
            cout<<"Invalid.\n\n";
            continue;
        }

        bool won = runLevel(levels[index]);
        cout << (won ? "Mission success.\n\n" : "Mission failed.\n\n");
    }

    return 0;
}
