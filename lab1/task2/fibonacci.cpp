#include <iostream>
#include <sys/time.h>
using namespace std;

int main() {
    int n, firstTerm = 1, secondTerm = 1, nextTerm;
    struct timeval start, end;

    cout << "Enter number of terms: ";
    cin >> n;

    gettimeofday(&start, NULL);

    // cout << "Fibonacci Series: " << firstTerm << " + " << secondTerm << " + ";

    //measure the execution time of the following loop
    for (int i = 1; i <= n-2; ++i) {
        nextTerm = firstTerm + secondTerm;
        // cout << nextTerm << " + ";
        firstTerm = secondTerm;
        secondTerm = nextTerm;
    }
    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    cout << "Execution time: " << elapsed << " seconds" << endl;

    return 0;
}
