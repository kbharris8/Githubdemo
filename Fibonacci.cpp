#include <iostream>
using namespace std;

static int recursiveCalls = 0;

int fibRecursive(int n) {
    recursiveCalls++;  

    if (n == 1 || n == 2)
        return 1;

    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

int fibLoop(int n) {
    if (n == 1 || n == 2)
        return 1;

    int a = 1, b = 1;

    for (int i = 3; i <= n; i++) {
        int temp = a + b;
        a = b;
        b = temp;
    }

    return b;
}

int main() {
    int n = 10;

    int recursiveResult = fibRecursive(n);
    cout << "Recursive fib(" << n << ") = " << recursiveResult << endl;
    cout << "Recursive calls: " << recursiveCalls << endl;

    int loopResult = fibLoop(n);
    cout << "Loop fib(" << n << ") = " << loopResult << endl;

    return 0;
}
