#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace std;

class Point {
private:
    double x, y;

public:
    // Constructor
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // Distance operator: p1 - p2
    double operator-(const Point& other) const {
        return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
    }

    // Equality operator
    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }

    // Inequality operator
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    // Midpoint operator: p1 / p2
    Point operator/(const Point& other) const {
        return Point((x + other.x) / 2.0, (y + other.y) / 2.0);
    }

    // Pre-increment: ++p  (increment x)
    Point& operator++() {
        x++;
        return *this;
    }

    // Post-increment: p++  (increment y)
    Point operator++(int) {
        Point temp = *this;
        y++;
        return temp;
    }

    // Pre-decrement: --p  (decrement x)
    Point& operator--() {
        x--;
        return *this;
    }

    // Post-decrement: p--  (decrement y)
    Point operator--(int) {
        Point temp = *this;
        y--;
        return temp;
    }

    // Index operator: p[0] = x, p[1] = y
    double& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        throw out_of_range("Index must be 0 or 1");
    }

    const double& operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        throw out_of_range("Index must be 0 or 1");
    }

    // Output stream operator: cout << p
    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    // Input stream operator: cin >> p  (expects format: (x, y)) 
    friend istream& operator>>(istream& is, Point& p) {
        char ch1, ch2, ch3;
        double xVal, yVal;

        is >> ch1 >> xVal >> ch2 >> yVal >> ch3;

        if (ch1 != '(' || ch2 != ',' || ch3 != ')') {
            is.setstate(ios::failbit);
        } else {
            p.x = xVal;
            p.y = yVal;
        }

        return is;
    }
};
