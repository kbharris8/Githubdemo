#include <iostream>
using namespace std;

int main() {
    char str[] = "hello world";
    char* p = str;


    while (*p != '\0')
        p++;

    
    p--;

    
    while (p >= str) {
        cout << *p;
        p--;
    }

    cout << endl;
    return 0;
}
