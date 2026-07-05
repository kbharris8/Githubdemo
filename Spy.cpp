#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

struct Position
{
    int row;
    int col;
};

struct Guard
{
    Position pos;
    char dir;
};

string toLowerCase(string text)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = tolower(text[i]);

    return text;
}

bool samePosition(Position a, Position b)
{
    return a.row == b.row && a.col == b.col;
}

Position getNextPosition(Position p, char move)
{
    if (move == 'w') p.row--;
    else if (move == 's') p.row++;
    else if (move == 'a') p.col--;
    else if (move == 'd') p.col++;

    return p;
}

char oppositeDirection(char dir)
{
    if (dir == '^') return 'v';
    if (dir == 'v') return '^';
    if (dir == '<') return '>';
    if (dir == '>') return '<';

    return dir;
}

char guardToMove(char dir)
{
    if (dir == '^') return 'w';
    if (dir == 'v') return 's';
    if (dir == '<') return 'a';
    if (dir == '>') return 'd';

    return ' ';
}

bool isGuardChar(char c)
{
    return c == '^' || c == 'v' || c == '<' || c == '>';
}

class Level
{
private:
    vector<string> map;
    vector<Guard> guards;
    Position player;
    Position goal;

public:
    Level(vector<string> newMap)
    {
        map = newMap;

        for (int r = 0; r < map.size(); r++)
        {
            for (int c = 0; c < map[r].length(); c++)
            {
                if (map[r][c] == '@')
                {
                    player = { r, c };
                    map[r][c] = ' ';
                }
                else if (map[r][c] == '$')
                {
                    goal = { r, c };
                }
                else if (isGuardChar(map[r][c]))
                {
                    Guard g;
                    g.pos = { r, c };
                    g.dir = map[r][c];
                    guards.push_back(g);
                    map[r][c] = ' ';
                }
            }
        }
    }

    void printMap()
    {
        vector<string> display = map;

        display[player.row][player.col] = '@';

        for (int i = 0; i < guards.size(); i++)
        {
            display[guards[i].pos.row][guards[i].pos.col] = guards[i].dir;
        }

        for (int r = 0; r < display.size(); r++)
        {
            cout << display[r] << endl;
        }
    }

    bool blocked(Position p)
    {
        if (map[p.row][p.col] == '#')
            return true;

        if (map[p.row][p.col] == '$')
            return true;

        if (samePosition(p, player))
            return true;

        for (int i = 0; i < guards.size(); i++)
        {
            if (samePosition(p, guards[i].pos))
                return true;
        }

        return false;
    }

    bool playerSeen()
    {
        for (int i = 0; i < guards.size(); i++)
        {
            Position check = guards[i].pos;

            while (true)
            {
                check = getNextPosition(check, guardToMove(guards[i].dir));

                if (map[check.row][check.col] == '#')
                    break;

                if (map[check.row][check.col] == '$')
                    break;

                if (samePosition(check, player))
                    return true;

                bool guardBlocking = false;

                for (int j = 0; j < guards.size(); j++)
                {
                    if (samePosition(check, guards[j].pos))
                    {
                        guardBlocking = true;
                        break;
                    }
                }

                if (guardBlocking)
                    break;
            }
        }

        return false;
    }

    void moveGuards()
    {
        for (int i = 0; i < guards.size(); i++)
        {
            Position next = getNextPosition(guards[i].pos, guardToMove(guards[i].dir));

            if (blocked(next))
            {
                guards[i].dir = oppositeDirection(guards[i].dir);
                next = getNextPosition(guards[i].pos, guardToMove(guards[i].dir));

                if (!blocked(next))
                    guards[i].pos = next;
            }
            else
            {
                guards[i].pos = next;
            }
        }
    }

    void play()
    {
        while (true)
        {
            printMap();

            if (playerSeen())
            {
                cout << "You were spotted by a guard. You lose!" << endl;
                return;
            }

            string input;
            cout << "Move with W/A/S/D: ";
            getline(cin, input);

            input = toLowerCase(input);

            if (input != "w" && input != "a" && input != "s" && input != "d")
            {
                cout << "Invalid input." << endl;
                continue;
            }

            Position next = getNextPosition(player, input[0]);

            if (map[next.row][next.col] == '#')
            {
                cout << "You hit a wall. Try again." << endl;
                continue;
            }

            for (int i = 0; i < guards.size(); i++)
            {
                if (samePosition(next, guards[i].pos))
                {
                    cout << "You ran into a guard. You lose!" << endl;
                    return;
                }
            }

            player = next;

            if (samePosition(player, goal))
            {
                printMap();
                cout << "You stole the information. You win!" << endl;
                return;
            }

            moveGuards();
        }
    }
};

int main()
{
    cout << "Welcome to Ultra-Spy!" << endl;
    cout << "Select a level:" << endl;
    cout << "1) A New Enemy" << endl;
    cout << "2) The Truth of the Weapon" << endl;

    string choice;
    getline(cin, choice);
    choice = toLowerCase(choice);

    vector<string> levelOne =
    {
        "########",
        "#    v #",
        "#>  #  #",
        "#   #  #",
        "#@  # $#",
        "########"
    };

    vector<string> levelTwo =
    {
        "##########",
        "#@     $ #",
        "#  ####  #",
        "#   <    #",
        "#   #  v #",
        "##########"
    };

    if (choice == "1" || choice == "a new enemy")
    {
        Level level(levelOne);
        level.play();
    }
    else if (choice == "2" || choice == "the truth of the weapon")
    {
        Level level(levelTwo);
        level.play();
    }
    else
    {
        cout << "Invalid level choice." << endl;
    }

    return 0;
}