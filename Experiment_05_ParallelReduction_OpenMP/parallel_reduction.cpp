#include <iostream>
#include <vector>
#include <omp.h>
#include <windows.h>

using namespace std;

double getTime() {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / freq.QuadPart;
}

int findMinSequential(vector<int>& a) {
    int mn = a[0];
    for (int i = 1; i < a.size(); i++) {
        if (a[i] < mn) {
            mn = a[i];
        }
    }
    return mn;
}

int findMaxSequential(vector<int>& a) {
    int mx = a[0];
    for (int i = 1; i < a.size(); i++) {
        if (a[i] > mx) {
            mx = a[i];
        }
    }
    return mx;
}

long long findSumSequential(vector<int>& a) {
    long long sum = 0;
    for (int i = 0; i < a.size(); i++) {
        sum += a[i];
    }
    return sum;
}

double findAverageSequential(vector<int>& a) {
    return (double)findSumSequential(a) / a.size();
}

int findMinParallel(vector<int>& a) {
    int mn = a[0];

    for (int i = 1; i < a.size(); i++) {
        if (a[i] < mn) {
            mn = a[i];
        }
    }

    return mn;
}

int findMaxParallel(vector<int>& a) {
    int mx = a[0];

    for (int i = 1; i < a.size(); i++) {
        if (a[i] > mx) {
            mx = a[i];
        }
    }

    return mx;
}

long long findSumParallel(vector<int>& a) {
    long long sum = 0;

    for (int i = 0; i < a.size(); i++) {
        sum += a[i];
    }

    return sum;
}

double findAverageParallel(vector<int>& a) {
    return (double)findSumParallel(a) / a.size();
}

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> a(n);

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    double start, end;
    double seqTime, parTime;

    start = getTime();
    int min1 = findMinSequential(a);
    int max1 = findMaxSequential(a);
    long long sum1 = findSumSequential(a);
    double avg1 = findAverageSequential(a);
    end = getTime();
    seqTime = (end - start) * 1000;

    cout << "\nSequential Results:" << endl;
    cout << "Min = " << min1 << endl;
    cout << "Max = " << max1 << endl;
    cout << "Sum = " << sum1 << endl;
    cout << "Average = " << avg1 << endl;
    cout << "Sequential Time: " << seqTime << " ms" << endl;

    start = getTime();
    int min2 = findMinParallel(a);
    int max2 = findMaxParallel(a);
    long long sum2 = findSumParallel(a);
    double avg2 = findAverageParallel(a);
    end = getTime();
    parTime = (end - start) * 1000;

    cout << "\nParallel Results:" << endl;
    cout << "Min = " << min2 << endl;
    cout << "Max = " << max2 << endl;
    cout << "Sum = " << sum2 << endl;
    cout << "Average = " << avg2 << endl;
    cout << "Parallel Time: " << parTime << " ms" << endl;

    if (parTime > 0) {
        cout << "\nSpeedup: " << seqTime / parTime << "x" << endl;
    }

    return 0;
}
