#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void bubbleSortSequential(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void bubbleSortParallel(vector<int>& a) {
    int n = a.size();

    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            for (int i = 0; i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    int temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        } else {
            for (int i = 1; i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    int temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        }
    }
}

void printArray(const vector<int>& a) {
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }
    cout << endl;
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

    vector<int> b = a;

    double start, end;
    double seqTime, parTime;

    start = omp_get_wtime();
    bubbleSortSequential(a);
    end = omp_get_wtime();
    seqTime = (end - start) * 1000;

    cout << "Sequential Sorted Array: ";
    printArray(a);
    cout << "Sequential Time: " << seqTime << " ms" << endl;

    start = omp_get_wtime();
    bubbleSortParallel(b);
    end = omp_get_wtime();
    parTime = (end - start) * 1000;

    cout << "Parallel Sorted Array: ";
    printArray(b);
    cout << "Parallel Time: " << parTime << " ms" << endl;

    if (parTime > 0) {
        cout << "Speedup: " << seqTime / parTime << "x" << endl;
    }

    return 0;
}
