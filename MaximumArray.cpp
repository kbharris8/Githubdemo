#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    const int SIZE = 10;
    int arr[SIZE];

    // fill with random values
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100;

    int* max_ptr = arr;        // start at first element
    int* current = arr;        // walker pointer
    int* end = arr + SIZE;     // one past last

    while (current < end) {
        if (*current > *max_ptr)
            max_ptr = current;
        current++;
    }

    cout << "Max value: " << *max_ptr << endl;
    return 0;
}
