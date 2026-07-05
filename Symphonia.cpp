#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Instrument {
protected:
    string name;
    string sound;

public:
    Instrument(string n, string s) : name(n), sound(s) {}
    virtual ~Instrument() {}

    virtual void play() const {
        cout << name << ": " << sound << endl;
    }
};

class Trumpet : public Instrument {
public:
    Trumpet() : Instrument("Trumpet", "doot") {}
    void play() const override {
        cout << name << ": ";
        for (int i = 0; i < 3; i++) cout << sound << " ";
        cout << endl;
    }
};

class Violin : public Instrument {
public:
    Violin() : Instrument("Violin", "vwa") {}
    void play() const override {
        cout << name << ": ";
        for (int i = 0; i < 5; i++) cout << sound << " ";
        cout << endl;
    }
};

class Piano : public Instrument {
public:
    Piano() : Instrument("Piano", "dee") {}
    void play() const override {
        cout << name << ": ";
        for (int i = 0; i < 2; i++) cout << sound << " ";
        cout << endl;
    }
};

class Trombone : public Instrument {
public:
    Trombone() : Instrument("Trombone", "Dwooooo~") {}
    void play() const override {
        cout << name << ": " << sound << endl;
    }
};

class Singer : public Instrument {
public:
    Singer() : Instrument("Singer", "blah") {}
    void play() const override {
        cout << name << ": ";
        for (int i = 0; i < 6; i++) cout << sound << " ";
        cout << endl;
    }
};

class Orchestra {
private:
    vector<Instrument*> instruments;

public:
    void add(Instrument* inst) {
        instruments.push_back(inst);
    }

    void play() const {
        for (auto inst : instruments) {
            inst->play();
        }
    }
};

int main() {
    Orchestra o;

    o.add(new Trumpet());
    o.add(new Violin());
    o.add(new Piano());
    o.add(new Trombone());
    o.add(new Trumpet());
    o.add(new Singer());

    o.play();

    return 0;
}
