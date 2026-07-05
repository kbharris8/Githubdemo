#include <iostream>
using namespace std;

template <typename T>
T max_two(const T& a, const T& b) {
    return (a > b) ? a : b;
}

template <typename T>
T max_three(const T& a, const T& b, const T& c) {
    return max_two(max_two(a, b), c);
}

template <typename T>
void swap_values(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 5, y = 10;
    cout << max_two(x, y) << endl;
    cout << max_three(3, 9, 7) << endl;

    swap_values(x, y);
    cout << x << " " << y << endl;

    return 0;
}
