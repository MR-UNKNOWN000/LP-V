/*
 * Experiment 11: Parallel Quicksort using MPI
 * VS Code Windows Version
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
// Standard quicksort for sorting within each process
void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partition: choose last element as pivot
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
        
        // Recursively sort left and right partitions
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// =================== Merge Two Sorted Arrays ===================
vector<int> mergeSorted(vector<int>& a, vector<int>& b) {
    vector<int> result;
    int i = 0, j = 0;
    
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) {
            result.push_back(a[i]);
            i++;
        } else {
            result.push_back(b[j]);
            j++;
        }
    }
    
    while (i < a.size()) {
        result.push_back(a[i]);
        i++;
    }
    while (j < b.size()) {
        result.push_back(b[j]);
        j++;
    }
    
    return result;
}

// =================== Print Array ===================
void printArray(vector<int>& arr, int limit = 20) {
    int n = arr.size();
    int printCount = min(n, limit);
    
    for (int i = 0; i < printCount; i++) {
        cout << arr[i] << " ";
    }
    if (n > limit) {
        cout << "... (" << n << " elements total)";
    }
    cout << endl;
}

// =================== Main Function ===================
int main(int argc, char* argv[]) {
    int rank, size;
    
    // ---- Initialize MPI ----
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get current process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes
    
    int n = 100000;  // Total array size
    vector<int> data;
    
    // ---- Process 0 (Master) generates the data ----
    if (rank == 0) {
        cout << "========================================" << endl;
        cout << " Parallel Quicksort using MPI" << endl;
        cout << "========================================" << endl;
        cout << "Array Size: " << n << endl;
        cout << "Number of MPI Processes: " << size << endl;
        
        // Generate random array
        srand(time(0));
        data.resize(n);
        for (int i = 0; i < n; i++) {
            data[i] = rand() % 100000;
        }
        
        cout << "\nOriginal Array (first 20): ";
        printArray(data);
        
        // ---- Sequential Quicksort for comparison ----
        vector<int> seq_data = data;  // Copy for sequential sort
        
        double seq_start = MPI_Wtime();
        quicksort(seq_data, 0, n - 1);
        double seq_end = MPI_Wtime();
        double seq_time = (seq_end - seq_start) * 1000;
        
        cout << "\nSequential Sorted (first 20): ";
        printArray(seq_data);
        cout << "Sequential Time: " << seq_time << " ms" << endl;
    }
    
    // ---- Parallel Quicksort using MPI ----
    MPI_Barrier(MPI_COMM_WORLD);  // Synchronize all processes
    double par_start = MPI_Wtime();
    
    // Step 1: Calculate chunk size for each process
    int chunk_size = n / size;
    int remainder = n % size;
    
    // Each process gets its chunk size
    int local_size;
    if (rank < remainder) {
        local_size = chunk_size + 1;
    } else {
        local_size = chunk_size;
    }
    
    // Allocate local array for each process
    vector<int> local_data(local_size);
    
    // Step 2: Scatter data from master to all processes
    // We'll use MPI_Scatterv for uneven distribution
    vector<int> sendcounts(size);
    vector<int> displs(size);
    
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            sendcounts[i] = (i < remainder) ? chunk_size + 1 : chunk_size;
            displs[i] = (i == 0) ? 0 : displs[i-1] + sendcounts[i-1];
        }
    }
    
    // Broadcast sendcounts and displs to all processes
    MPI_Bcast(sendcounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displs.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Scatter: distribute chunks to all processes
    MPI_Scatterv(data.data(), sendcounts.data(), displs.data(), MPI_INT,
                 local_data.data(), local_size, MPI_INT,
                 0, MPI_COMM_WORLD);
    
    // Step 3: Each process sorts its chunk locally
    quicksort(local_data, 0, local_size - 1);
    
    // Step 4: Gather sorted chunks back to master
    // First, gather the sizes
    vector<int> recvcounts(size);
    MPI_Gather(&local_size, 1, MPI_INT, recvcounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Calculate displacements for gathering
    vector<int> recvdispls(size);
    if (rank == 0) {
        recvdispls[0] = 0;
        for (int i = 1; i < size; i++) {
            recvdispls[i] = recvdispls[i-1] + recvcounts[i-1];
        }
    }
    
    // Gather all sorted chunks
    vector<int> gathered_data;
    if (rank == 0) {
        gathered_data.resize(n);
    }
    
    MPI_Gatherv(local_data.data(), local_size, MPI_INT,
                gathered_data.data(), recvcounts.data(), recvdispls.data(), MPI_INT,
                0, MPI_COMM_WORLD);
    
    // Step 5: Master merges all sorted chunks
    if (rank == 0) {
        // Merge sorted chunks
        vector<int> result;
        int start = 0;
        
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
        
        // ---- Performance Comparison ----
        // Recalculate sequential time for fair comparison
        vector<int> seq_data2 = data;
        double s1 = MPI_Wtime();
        quicksort(seq_data2, 0, n - 1);
        double s2 = MPI_Wtime();
        double seq_time2 = (s2 - s1) * 1000;
        
        cout << "\n========================================" << endl;
        cout << " Performance Comparison" << endl;
        cout << "========================================" << endl;
        cout << "Sequential Time:             " << seq_time2 << " ms" << endl;
        cout << "Parallel Time (" << size << " processes): " << par_time << " ms" << endl;
        if (par_time > 0) {
            cout << "Speedup:                     " << seq_time2 / par_time << "x" << endl;
        }
        
        // Verify correctness
        bool correct = (result.size() == seq_data2.size());
        if (correct) {
            for (int i = 0; i < n; i++) {
                if (result[i] != seq_data2[i]) {
                    correct = false;
                    break;
                }
            }
        }
        cout << "Verification: " << (correct ? "PASSED" : "FAILED") << endl;
        
        cout << "========================================" << endl;
        cout << " MPI Quicksort Completed!" << endl;
        cout << "========================================" << endl;
    }
    
    // ---- Finalize MPI ----
    MPI_Finalize();
    return 0;
}
