#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Hero {
private:
    int strength;       
    int courage;        
    string name;        

public:
    
    Hero(string heroName) {
        name = heroName;
        strength = rand() % 51 + 50; 
        courage = rand() % 51 + 50;  
    }

    
    int getStrength() const { return strength; }
    int getCourage() const { return courage; }
    string getName() const { return name; }

    
    void setCourage(int c) { courage = c; }

    void train(int hours) {
        strength += hours * 2;   
        cout << name << " trained for " << hours 
             << " hours. Strength is now " << strength << endl;
    }

    void therapy() {
        int boost = rand() % 11 + 5; 
        courage += boost;
        cout << name << " attended therapy and gained " 
             << boost << " courage. Courage is now " << courage << endl;
    }
};


void sendOnQuest(Hero &h) {   
    cout << "\nSending " << h.getName() << " on a quest...\n";

    int roll = rand() % 100 + 1;  // 1–100
    int strength = h.getStrength();

    string result;

    if (roll <= strength * 0.4) {
        result = "SUCCESS";
        h.setCourage(h.getCourage() - 5);
    }
    else if (roll <= strength * 0.8) {
        result = "NEUTRAL OUTCOME";
        h.setCourage(h.getCourage() - 10);
    }
    else {
        result = "FAILURE";
        h.setCourage(h.getCourage() - 20);
    }

    cout << h.getName() << " quest result: " << result << endl;
    cout << "Courage is now " << h.getCourage() << endl;
}


int main() {
    srand(time(0)); 

    Hero h1("Astra");
    Hero h2("Blaze");
    Hero h3("Orion");

    cout << "Initial Stats:\n";
    cout << h1.getName() << " - Strength: " << h1.getStrength() 
         << ", Courage: " << h1.getCourage() << endl;
    cout << h2.getName() << " - Strength: " << h2.getStrength() 
         << ", Courage: " << h2.getCourage() << endl;
    cout << h3.getName() << " - Strength: " << h3.getStrength() 
         << ", Courage: " << h3.getCourage() << endl;

    h1.train(3);
    h2.train(5);
    h3.therapy();

    sendOnQuest(h1);
    sendOnQuest(h2);
    sendOnQuest(h3);

    return 0;
}
