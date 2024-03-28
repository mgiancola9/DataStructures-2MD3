#include <cstdlib>
#include <iostream>

using namespace std;

int LLArray() {
    //declare variables for user input of the first line
    int n, first, x;

    cout << "Please input num of elements in list, index of first element, and integer of min value:" << endl;
    cin >> n >> first >> x;

    int value, next;
    int array[2][n];    //creates a 2 by n array
    cout << "please input current element's value, and element it is pointing to, " << n << "times:" << endl;
    //for loop to check the value of the node and where it is pointing to
    for (int i = 0; i < n; i++) {
        cin >> value >> next;

        array[0][i] = value;    //adds value to the array
        array[1][i] = next;     //adds the value pointed to the array in another row
    }

    int min = -1;
    int cur = first;
    //do while loop to check for the min value that's greater than or equal to x, if -1 there's no such number
    do {
        //if index of array is greater than or equal to x and is less than min or the min equals -1, stay in same row
        if (array[0][cur-1] >= x && (array[0][cur-1] < min || min == -1)) {
            min = array[0][cur-1];
        }
        //if conditions above are not met, move to next row
        cur = array[1][cur-1];

    } while (cur != -1); 

    return min;
}

int main() {
  int val = LLArray();
  cout << "the min value is: " << val << endl;
}