#include <iostream>
#include <functional>
#include <cassert>

using namespace std;

int until(int x, function<int(int)> f, function<bool(int)> g) {
    while (!g(x)) {
        x = f(x);
    }
    return x;
}

int main() {
   
    int r1 = until(1, [](int x){ return x * 2; }, [](int x){ return x > 100; });
    cout << "Test 1: " << r1 << endl;
    assert(r1 == 128);

    int r2 = until(1, [](int x){ return x + 1; }, [](int x){ return x == 5; });
    cout << "Test 2: " << r2 << endl;
    assert(r2 == 5);

    int r3 = until(200, [](int x){ return x * 2; }, [](int x){ return x > 100; });
    cout << "Test 3: " << r3 << endl;
    assert(r3 == 200);

    int r4 = until(10, [](int x){ return x - 3; }, [](int x){ return x <= 0; });
    cout << "Test 4: " << r4 << endl;
    assert(r4 == -2);

    
    int r5 = until(2, [](int x){ return x * x; }, [](int x){ return x > 1000; });
    cout << "Test 5: " << r5 << endl;
    assert(r5 == 65536);

    
    int r6 = until(-5, [](int x){ return -x; }, [](int x){ return x > 0; });
    cout << "Test 6: " << r6 << endl;
    assert(r6 == 5);

}