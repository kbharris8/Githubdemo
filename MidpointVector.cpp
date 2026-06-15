#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {5, 9, 2, 7, 1, 8};  // example data

    int* left = &v[0];
    int* right = &v[v.size() - 1];

    while (left < right) {
        left++;
        right--;
    }

    // If even number of elements, left passes right
    // Return the smaller index (left - 1)
    int midpoint_value;

    if (left == right)
        midpoint_value = *left;
    else
        midpoint_value = *(left - 1);

    cout << "Midpoint value: " << midpoint_value << endl;

    return 0;
}
