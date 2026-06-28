#include "Unicorn.h"
#include <iostream>

std::set<std::string> Unicorn::usedNames;

Unicorn::Unicorn(const std::string& n) {
    if (usedNames.count(n)) {
        std::cout << "Error: The name \"" << n << "\" is already taken.\n";
        name = ""; // invalid unicorn
    } else {
        name = n;
        usedNames.insert(n);
        std::cout << "A unicorn named " << name << " has been created.\n";
    }
}

Unicorn::~Unicorn() {
    if (!name.empty()) {
        std::cout << name << " has lost its magic. Name freed.\n";
        usedNames.erase(name);
    }
}
