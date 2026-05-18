# MPI Quicksort - Ubuntu + VS Code + OpenMPI Setup

## Aim
Evaluate performance enhancement of parallel Quicksort Algorithm using MPI on Ubuntu with OpenMPI.

## Objective
- Understand MPI programming model: processes, communication, synchronization
- Learn MPI functions: Init, Scatter, Gather, Barrier, Finalize
- Implement parallel Quicksort by distributing sorting work across processes
- Measure performance improvement with varying number of processes
- Understand communication overhead vs computation benefit tradeoff
- Compare sequential vs parallel execution time

## Ubuntu Setup Instructions

### Step 1: Update System
```bash
sudo apt update
```

### Step 2: Install OpenMPI
```bash
sudo apt install openmpi-bin openmpi-common libopenmpi-dev
```

### Step 3: Verify Installation
```bash
mpirun --version
mpic++ --version
```

### Step 4: Compile
```bash
mpic++ quicksort_mpi.cpp -o quicksort
```

### Step 5: Run with 4 processes
```bash
mpirun -np 4 ./quicksort
```

### Step 6: Run with different process counts
```bash
mpirun -np 2 ./quicksort
mpirun -np 8 ./quicksort
```

## VS Code Setup on Ubuntu
1. Install VS Code: `sudo snap install code --classic`
2. Install C/C++ extension
3. Open terminal in VS Code (Ctrl+`)
4. Compile and run using commands above

## Theory

### MPI Communication Model
```
Process 0 (Master)          Process 1          Process 2          Process 3
    |                           |                  |                  |
    |--- MPI_Scatter --------->|                  |                  |
    |--- MPI_Scatter --------------------------------->|              |
    |--- MPI_Scatter ------------------------------------------------>|
    |                           |                  |                  |
  Sort                        Sort               Sort               Sort
    |                           |                  |                  |
    |<-- MPI_Gather ------------|                  |                  |
    |<-- MPI_Gather ----------------------------------|                |
    |<-- MPI_Gather --------------------------------------------------|
    |
  Merge All
  Print Result
```

### MPI Functions Used
| Function | Purpose |
|----------|---------|
| `MPI_Init` | Initialize MPI |
| `MPI_Comm_rank` | Get process ID |
| `MPI_Comm_size` | Get total processes |
| `MPI_Scatterv` | Distribute variable-sized chunks |
| `MPI_Gatherv` | Collect variable-sized chunks |
| `MPI_Bcast` | Broadcast to all processes |
| `MPI_Barrier` | Synchronize processes |
| `MPI_Wtime` | Get wall clock time |
| `MPI_Finalize` | Clean up MPI |

### Performance Improvement
| Processes | Expected Speedup |
|-----------|-----------------|
| 1 | 1.0x (baseline) |
| 2 | ~1.8x |
| 4 | ~3.2x |
| 8 | ~5.5x |

> Speedup is not perfectly linear due to communication overhead and merge step.

## Algorithm

```
PARALLEL QUICKSORT WITH MPI:

1. Process 0 (Master):
   - Generate random array of size n
   - Record start time

2. Scatter Phase:
   - MPI_Scatterv: Distribute chunks to all processes
   - Process 0 gets chunk[0...n/p)
   - Process 1 gets chunk[n/p...2n/p)
   - Process i gets chunk[i*n/p...(i+1)*n/p)

3. Local Sort Phase (Each Process):
   - Perform sequential Quicksort on assigned chunk
   - No communication needed during local sort

4. Gather Phase:
   - MPI_Gatherv: All processes send sorted chunks to Master
   - Master collects all sorted chunks in order

5. Master Merge Phase:
   - Merge all sorted chunks into final sorted array
   - (Since chunks from Scatter are already in order, merge is simple concatenation)

6. Record end time and calculate speedup
   - Speedup = Sequential Time / Parallel Time

7. MPI_Finalize: Clean up MPI environment
```

## Code Explanation

### MPI Initialization:
```cpp
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
```
- `MPI_Init`: Initialize MPI environment
- `Comm_rank`: Get this process's unique ID (0 to size-1)
- `Comm_size`: Get total number of processes

### Array Scattering:
```cpp
MPI_Scatterv(array, sendcounts, displs, MPI_INT, 
             local_array, recvcount, MPI_INT, 0, MPI_COMM_WORLD);
```
- Distributes different chunk sizes from master to all processes
- `sendcounts[]`: size of each chunk
- `displs[]`: starting position of each chunk in original array
- Each process receives its chunk in `local_array`

### Local Sorting:
```cpp
quicksort(local_array, 0, local_size - 1);
```
- Each process sorts its local chunk independently using sequential Quicksort
- No inter-process communication needed here

### Array Gathering:
```cpp
MPI_Gatherv(local_array, local_size, MPI_INT,
            array, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
```
- All processes send sorted chunks back to master
- Master reassembles in correct order
- Result is fully sorted array

### Synchronization:
```cpp
MPI_Barrier(MPI_COMM_WORLD);
```
- All processes wait here until everyone arrives
- Used for accurate timing measurements

### Timing:
```cpp
double start = MPI_Wtime();
// ... computation ...
double end = MPI_Wtime();
```
- `MPI_Wtime()` returns wall-clock time; allows accurate performance measurement

## Expected Output
```
========================================
 MPI Parallel Quicksort
========================================
Enter array size: 1000000
Number of processes: 4

--- Sequential Quicksort ---
Time: 2345.67 ms
First 10 sorted elements: 0 1 2 3 4 5 6 7 8 9

--- Parallel Quicksort (4 processes) ---
Time: 742.45 ms

========================================
 Performance Summary
========================================
Array Size: 1,000,000 elements
Number of Processes: 4
Sequential Time: 2345.67 ms
Parallel Time:   742.45 ms
Speedup:         3.16x
Efficiency:      79.0%

Process Distribution:
  Process 0: 250,000 elements → 587.12 ms
  Process 1: 250,000 elements → 589.43 ms
  Process 2: 250,000 elements → 588.67 ms
  Process 3: 250,000 elements → 590.21 ms

Verification: PASSED (array correctly sorted)
========================================
```

## Advantages
1. **Linear Speedup Potential**: Sorting embarrassingly parallel; minimal communication overhead
2. **Scalability**: Works well with increasing number of processes
3. **Distributed Memory**: Can work across multiple machines unlike OpenMP
4. **Load Balancing**: Can adjust chunk sizes for balanced processing
5. **No Shared Memory Conflicts**: Each process has independent data; no race conditions

## Applications
1. **Large-Scale Data Sorting**: Sorting massive datasets across compute clusters
2. **Distributed Databases**: Query result set sorting in distributed systems
3. **MapReduce Framework**: Shuffle phase in Hadoop uses similar scatter-gather
4. **Parallel Data Analytics**: Processing big data across multiple nodes
5. **Scientific Computing**: Sorting intermediate results in parallel simulations
6. **Grid Computing**: Leveraging multiple workstations for sorting tasks

## Viva Questions

Please refer to VSCode/README.md for complete viva questions and answers (same for both setups).

## Conclusion
Using OpenMPI on Ubuntu provides a straightforward way to compile and run MPI programs. The parallel Quicksort demonstrates significant speedup by distributing sorting work across multiple processes.
