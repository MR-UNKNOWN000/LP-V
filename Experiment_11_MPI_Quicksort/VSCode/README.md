# MPI Quicksort - VS Code Windows Setup

## Aim
Evaluate performance enhancement of parallel Quicksort Algorithm using MPI.

## Objective
- Understand MPI programming model: processes, communication, synchronization
- Learn MPI functions: Init, Scatter, Gather, Barrier, Finalize
- Implement parallel Quicksort by distributing sorting work across processes
- Measure performance improvement with varying number of processes
- Understand communication overhead vs computation benefit tradeoff
- Compare sequential vs parallel execution time

## VS Code Windows Setup

### Step 1: Install MS-MPI
1. Download MS-MPI from: https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi
2. Install both:
   - `msmpisetup.exe` (runtime)
   - `msmpisdk.msi` (SDK)
3. Add to PATH:
   - `C:\Program Files\Microsoft MPI\Bin`
   - `C:\Program Files (x86)\Microsoft SDKs\MPI\Include`
   - `C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64`

### Step 2: VS Code Setup
1. Install C/C++ extension
2. Install Code Runner extension

### Step 3: Compile & Run
See Commands.txt for compilation and execution commands.

## Theory

### What is MPI?
Message Passing Interface (MPI) is a standard for parallel programming where multiple processes communicate by **sending and receiving messages**. Unlike OpenMP (shared memory), MPI works across different machines.

### Key MPI Concepts
| Function | Description |
|----------|-------------|
| `MPI_Init` | Initialize MPI environment |
| `MPI_Comm_rank` | Get process ID (rank) |
| `MPI_Comm_size` | Get total number of processes |
| `MPI_Scatter` | Distribute data from root to all |
| `MPI_Gather` | Collect data from all to root |
| `MPI_Bcast` | Broadcast data from root to all |
| `MPI_Barrier` | Synchronize all processes |
| `MPI_Finalize` | Clean up MPI environment |

### Parallel Quicksort with MPI
```
1. Master (rank 0) generates random array
2. Scatter: Master distributes chunks to all processes
3. Each process sorts its chunk using quicksort
4. Gather: All processes send sorted chunks back to master
5. Master merges all sorted chunks
```

### MPI Process Communication
```
                 Master (Rank 0)
                 [Full Array]
                      |
         MPI_Scatter (distribute)
        /      |       |      \
   Rank 0   Rank 1   Rank 2   Rank 3
   [chunk]  [chunk]   [chunk]  [chunk]
      |        |         |        |
   Sort     Sort      Sort     Sort
      |        |         |        |
        \      |       |      /
         MPI_Gather (collect)
                      |
                 Master (Rank 0)
                 [Merge All]
```

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

## Viva Questions and Answers

**Q1: What is MPI?**
A: MPI (Message Passing Interface) is a standard for parallel programming using message passing between processes. It works across distributed systems.

**Q2: What is the difference between MPI and OpenMP?**
A: MPI uses distributed memory (processes communicate via messages). OpenMP uses shared memory (threads share variables). MPI can work across multiple machines.

**Q3: What is MPI_Scatter?**
A: It distributes equal chunks of data from root process to all processes in the communicator.

**Q4: What is MPI_Gather?**
A: It collects data from all processes and assembles it at the root process.

**Q5: What is a rank in MPI?**
A: The unique ID assigned to each process. Root process is usually rank 0.

**Q6: Why merge after gathering?**
A: Each process sorts its own chunk. After gathering, the chunks are individually sorted but not merged. We need a final merge step.

**Q7: What is MPI_Comm_size?**
A: Returns the total number of processes in the communicator.

**Q8: What is MPI_Barrier?**
A: Synchronizes all processes. No process continues until all have reached the barrier.

**Q9: What is the time complexity of parallel quicksort?**
A: O(n log n / p + n/p * log(n/p)) where p = number of processes.

**Q10: What is MPI_Wtime?**
A: Returns wall clock time in seconds. Used for performance measurement.

## Conclusion
We successfully evaluated the performance enhancement of parallel Quicksort using MPI. The Scatter-Sort-Gather approach distributes work across processes, achieving speedup proportional to the number of processes.
