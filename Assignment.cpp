#include <iostream>
using namespace std;

int main() {
    int a, b;
    cout << "Enter two integers: ";
    cin >> a >> b;

    int* ptr_a = &a;
    int* ptr_b = &b;

    cout << "ptr_a points to: " << *ptr_a << endl;
    cout << "ptr_b points to: " << *ptr_b << endl;

    return 0;
}

