#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
using namespace std;

template <typename T>
vector<T> vec_fill(T elem) {
    vector<T> v(10);
    for (int i = 0; i < 10; i++) {
        v[i] = elem + i;
    }
    return v;
}

template <typename T>
T until(T elem, function<T(T)> func, function<bool(T)> predicate) {
    while (!predicate(elem)) {
        elem = func(elem);
    }
    return elem;
}

template <typename T>
void swap_vals(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void order(T& a, T& b, T& c) {
    if (a < b) swap_vals(a, b);
    if (a < c) swap_vals(a, c);
    if (b < c) swap_vals(b, c);
}

void test_vec_fill() {
    
    vector<int> v1 = vec_fill(5);
    assert(v1[0] == 5 && v1[9] == 14);

    vector<double> v2 = vec_fill(2.5);
    assert(v2[0] == 2.5 && v2[9] == 11.5);

    vector<char> v3 = vec_fill('A');
    assert(v3[0] == 'A' && v3[9] == 'J');
}

void test_until() {
    
    int r1 = until<int>(3, 
        [](int x){ return x + 1; }, 
        [](int x){ return x >= 10; }
    );
    assert(r1 == 10);

    double r2 = until<double>(2.0, 
        [](double x){ return x * 2; }, 
        [](double x){ return x > 100; }
    );
    assert(r2 == 128.0);

    string r3 = until<string>("hi",
        [](string s){ return s + "!"; },
        [](string s){ return s.size() >= 5; }
    );
    assert(r3 == "hi!!!");
}

void test_order() {
    
    int a1 = 3, b1 = 10, c1 = 7;
    order(a1, b1, c1);
    assert(a1 == 10 && b1 == 7 && c1 == 3);

    double a2 = 1.1, b2 = 5.5, c2 = 3.3;
    order(a2, b2, c2);
    assert(a2 == 5.5 && b2 == 3.3 && c2 == 1.1);

    char a3 = 'b', b3 = 'z', c3 = 'm';
    order(a3, b3, c3);
    assert(a3 == 'z' && b3 == 'm' && c3 == 'b');
}

int main() {
    test_vec_fill();
    test_until();
    test_order();

    cout << "The test was passed" << endl;
    return 0;
}
