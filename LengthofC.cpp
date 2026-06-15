#include <iostream>
using namespace std;

int main() {
    char str[] = "hello world";
    char* p = str;

    int length = 0;
    while (*p != '\0') {
        length++;
        p++;
    }

    cout << "Length: " << length << endl;
    return 0;
}
