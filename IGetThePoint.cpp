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

    // Distance operator
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

    // Midpoint operator
    Point operator/(const Point& other) const {
        return Point((x + other.x) / 2.0, (y + other.y) / 2.0);
    }

    // Pre-increment
    Point& operator++() {
        x++;
        return *this;
    }

    // Post-increment
    Point operator++(int) {
        Point temp = *this;
        y++;
        return temp;
    }

    // Pre-decrement
    Point& operator--() {
        x--;
        return *this;
    }

    // Post-decrement
    Point operator--(int) {
        Point temp = *this;
        y--;
        return temp;
    }

    // Index operator
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

    // Output stream operator
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
