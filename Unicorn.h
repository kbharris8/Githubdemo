#ifndef UNICORN_H
#define UNICORN_H

#include <string>
#include <set>

class Unicorn {
private:
    std::string name;
    static std::set<std::string> usedNames;

public:
    Unicorn(const std::string& n);
    ~Unicorn();
};

#endif
