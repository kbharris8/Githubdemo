#include <iostream>
#include "IGetThePoint.cpp"
using namespace std;

int main() {
    Point p1(2, 4);
    Point p2(6, 10);

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;

    cout << "Distance (p1 - p2): " << (p1 - p2) << endl;
    cout << "Midpoint (p1 / p2): " << (p1 / p2) << endl;

    cout << "p1 == p2: " << (p1 == p2) << endl;
    cout << "p1 != p2: " << (p1 != p2) << endl;

    cout << "++p1: " << ++p1 << endl;
    cout << "p1++: " << p1++ << endl;
    cout << "After p1++: " << p1 << endl;

    cout << "--p1: " << --p1 << endl;
    cout << "p1--: " << p1-- << endl;
    cout << "After p1--: " << p1 << endl;

    cout << "p1[0] = " << p1[0] << endl;
    cout << "p1[1] = " << p1[1] << endl;

    Point p3;
    cout << "Enter a point like (3, 7): ";
    cin >> p3;
    cout << "You entered: " << p3 << endl;

    return 0;
}

