#include <iostream>
#include <cmath>
using namespace std;

class Shape {
public:
    virtual double getArea() const = 0;        
    virtual double getPerimeter() const = 0;   
    virtual string getName() const = 0;        

    virtual ~Shape() {} 
};


class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double getArea() const override {
        return M_PI * radius * radius;
    }

    double getPerimeter() const override {
        return 2 * M_PI * radius;
    }

    string getName() const override {
        return "Circle";
    }
};

class Rectangle : public Shape {
protected:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double getArea() const override {
        return width * height;
    }

    double getPerimeter() const override {
        return 2 * (width + height);
    }

    string getName() const override {
        return "Rectangle";
    }
};

class Square : public Rectangle {
public:
    Square(double side) : Rectangle(side, side) {}

    string getName() const override {
        return "Square";
    }
};

class RightTriangle : public Shape {
protected:
    double base;
    double height;

public:
    RightTriangle(double b, double h) : base(b), height(h) {}

    double getArea() const override {
        return 0.5 * base * height;
    }

    double getPerimeter() const override {
        double hyp = sqrt(base*base + height*height);
        return base + height + hyp;
    }

    string getName() const override {
        return "Right Triangle";
    }
};

class IsoscelesRightTriangle : public RightTriangle {
public:
    IsoscelesRightTriangle(double leg)
        : RightTriangle(leg, leg) {}

    string getName() const override {
        return "Isosceles Right Triangle";
    }
};

void printAreaToScreen(Shape *s) {
    cout << "The area of the " << s->getName()
         << " is: " << s->getArea() << endl;
}

int main() {
    Circle c(5);
    Rectangle r(4, 6);
    Square sq(4);
    RightTriangle rt(3, 4);
    IsoscelesRightTriangle irt(5);

    printAreaToScreen(&c);
    printAreaToScreen(&r);
    printAreaToScreen(&sq);
    printAreaToScreen(&rt);
    printAreaToScreen(&irt);

    return 0;
}
