/*
 * Experiment 11: Parallel Quicksort using MPI
 * Ubuntu + VS Code + OpenMPI Version
 * 
 * Description: Implements parallel Quicksort using MPI (Message Passing Interface)
 * to evaluate performance enhancement over sequential Quicksort.
 * 
 * Compile: mpic++ quicksort_mpi.cpp -o quicksort
 * Run: mpirun -np 4 ./quicksort
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <mpi.h>

using namespace std;

// =================== Sequential Quicksort ===================
void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partition using last element as pivot
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// =================== Merge Two Sorted Arrays ===================
vector<int> mergeSorted(vector<int>& a, vector<int>& b) {
    vector<int> result;
    int i = 0, j = 0;
    
    while (i < (int)a.size() && j < (int)b.size()) {
        if (a[i] <= b[j]) {
            result.push_back(a[i++]);
        } else {
            result.push_back(b[j++]);
        }
    }
    while (i < (int)a.size()) result.push_back(a[i++]);
    while (j < (int)b.size()) result.push_back(b[j++]);
    
    return result;
}

// =================== Print Array ===================
void printArray(vector<int>& arr, int limit = 20) {
    int n = arr.size();
    int printCount = min(n, limit);
    for (int i = 0; i < printCount; i++) cout << arr[i] << " ";
    if (n > limit) cout << "... (" << n << " total)";
    cout << endl;
}

// =================== Main Function ===================
int main(int argc, char* argv[]) {
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int n = 100000;
    vector<int> data;
    
    if (rank == 0) {
        cout << "========================================" << endl;
        cout << " Parallel Quicksort using MPI (Ubuntu)" << endl;
        cout << "========================================" << endl;
        cout << "Array Size: " << n << endl;
        cout << "Number of MPI Processes: " << size << endl;
        
        srand(time(0));
        data.resize(n);
        for (int i = 0; i < n; i++) data[i] = rand() % 100000;
        
        cout << "\nOriginal Array (first 20): ";
        printArray(data);
        
        // Sequential sort for comparison
        vector<int> seq_data = data;
        double seq_start = MPI_Wtime();
        quicksort(seq_data, 0, n - 1);
        double seq_end = MPI_Wtime();
        cout << "Sequential Time: " << (seq_end - seq_start) * 1000 << " ms" << endl;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    double par_start = MPI_Wtime();
    
    // Calculate chunk sizes
    int chunk_size = n / size;
    int remainder = n % size;
    int local_size = (rank < remainder) ? chunk_size + 1 : chunk_size;
    
    vector<int> local_data(local_size);
    
    // Prepare scatter parameters
    vector<int> sendcounts(size), displs(size);
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            sendcounts[i] = (i < remainder) ? chunk_size + 1 : chunk_size;
            displs[i] = (i == 0) ? 0 : displs[i-1] + sendcounts[i-1];
        }
    }
    
    MPI_Bcast(sendcounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displs.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Scatter data
    MPI_Scatterv(data.data(), sendcounts.data(), displs.data(), MPI_INT,
                 local_data.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Sort local chunk
    quicksort(local_data, 0, local_size - 1);
    
    // Gather sorted chunks
    vector<int> recvcounts(size);
    MPI_Gather(&local_size, 1, MPI_INT, recvcounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    vector<int> recvdispls(size);
    vector<int> gathered_data;
    if (rank == 0) {
        recvdispls[0] = 0;
        for (int i = 1; i < size; i++)
            recvdispls[i] = recvdispls[i-1] + recvcounts[i-1];
        gathered_data.resize(n);
    }
    
    MPI_Gatherv(local_data.data(), local_size, MPI_INT,
                gathered_data.data(), recvcounts.data(), recvdispls.data(), MPI_INT,
                0, MPI_COMM_WORLD);
    
    // Merge sorted chunks
    if (rank == 0) {
        vector<int> result;
        for (int i = 0; i < size; i++) {
            vector<int> chunk(gathered_data.begin() + recvdispls[i],
                            gathered_data.begin() + recvdispls[i] + recvcounts[i]);
            result = mergeSorted(result, chunk);
        }
        
        double par_end = MPI_Wtime();
        double par_time = (par_end - par_start) * 1000;
        
        cout << "\nParallel Sorted (first 20): ";
        printArray(result);
        cout << "Parallel Time (" << size << " processes): " << par_time << " ms" << endl;
        
        cout << "\n========================================" << endl;
        cout << " MPI Quicksort Completed Successfully!" << endl;
        cout << "========================================" << endl;
    }
    
    MPI_Finalize();
    return 0;
}
