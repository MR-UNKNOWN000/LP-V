#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void merge(vector<int>& a, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = a[left + i];
    }

    for (int i = 0; i < n2; i++) {
        R[i] = a[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        } else {
            a[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        a[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        a[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortSequential(vector<int>& a, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        mergeSortSequential(a, left, mid);
        mergeSortSequential(a, mid + 1, right);

        merge(a, left, mid, right);
    }
}

void mergeSortParallel(vector<int>& a, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = (left + right) / 2;

        if (depth < 3) {
            {
                mergeSortParallel(a, left, mid, depth + 1);

                mergeSortParallel(a, mid + 1, right, depth + 1);
            }
        } else {
            mergeSortSequential(a, left, mid);
            mergeSortSequential(a, mid + 1, right);
        }

        merge(a, left, mid, right);
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
    mergeSortSequential(a, 0, n - 1);
    end = omp_get_wtime();
    seqTime = (end - start) * 1000;

    cout << "Sequential Sorted Array: ";
    printArray(a);
    cout << "Sequential Time: " << seqTime << " ms" << endl;

    start = omp_get_wtime();
    mergeSortParallel(b, 0, n - 1);
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
