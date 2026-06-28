#include <iostream>
#include <string>
#include <set>

class Unicorn {
private:
    std::string name;
    bool valid;
    static std::set<std::string> usedNames;

public:
    Unicorn(const std::string& n) : name(n), valid(false) {
        if (usedNames.count(name)) {
            std::cout << "Error: A unicorn named \"" << name << "\" already exists!\n";
        } else {
            usedNames.insert(name);
            valid = true;
            std::cout << "Unicorn \"" << name << "\" created.\n";
        }
    }

    ~Unicorn() {
        if (valid) {
            usedNames.erase(name);
            std::cout << "Unicorn \"" << name << "\" destroyed. Name is now free.\n";
        }
    }

    std::string getName() const { return name; }
    bool isValid() const { return valid; }
};

std::set<std::string> Unicorn::usedNames;

int main() {
    Unicorn u1("James");      
    Unicorn u2("Lily");     
    Unicorn u3("James");       
    {
        Unicorn u4("Ken"); 
    }                           
    Unicorn u5("Ken");      
}
