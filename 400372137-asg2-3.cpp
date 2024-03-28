#include <iostream>
#include <vector>

using namespace std;

// tail recursive function that finds both the min and max values in a vector
void minMax(vector<int>& A, int& min, int& max, int i) {
    // base case, if we have reached the end of the vector, return the current min and max
    if (i == A.size()) 
    {
        return;
    }
    // If the current value is less than the current min, update min
    if (A[i] < min) 
    {
        min = A[i];
    }
    // If current value is greater than the current max, update max
    if (A[i] > max) 
    {
        max = A[i];
    }
    // Recursive call to next i value(index)
    minMax(A, min, max, i + 1);
}

int main() {
    vector<int> A = {1, -2, 0, 6, 66, -7};
    // Initialize min and max to first value of the vector
    int min = A[0];
    int max = A[0];
    minMax(A, min, max, 2);
    cout << "Min " << min << " Max " << max << endl;
    return 0;
}