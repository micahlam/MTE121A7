#include <iostream>
using namespace std;


void swapInts(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 12;
    int y = 11;

    cout << "Before swap: x = " << x << ", y = " << y << endl;

    swapInts(&x, &y);

    cout << "After swap:  x = " << x << ", y = " << y << endl;

    return 0;
}

/*
--------------------------------------------
Test Case 1:
Input: x = 12, y = 11
Output:
Before swap: x = 12, y = 11
After swap:  x = 11, y = 12

Test Case 2:
Input: x = -5, y = 42
Output:
Before swap: x = -5, y = 42
After swap:  x = 42, y = -5
--------------------------------------------
*/