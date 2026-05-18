# Experiment 01: Parallel Breadth First Search (BFS) using OpenMP

## Aim
Design and implement Parallel Breadth First Search based on existing algorithms using OpenMP. Use a Tree or an undirected graph for BFS.

## Objective
- Understand BFS algorithm and its level-based structure
- Learn OpenMP directives: `#pragma omp parallel for` and `#pragma omp critical`
- Implement thread-safe graph traversal using critical sections
- Compare sequential vs parallel execution time and identify performance improvements
- Recognize when parallel BFS is beneficial (large graphs) vs when overhead dominates (small graphs)

## Theory

### What is BFS?
Breadth First Search (BFS) is a graph traversal algorithm that visits all nodes at the current depth level before moving to nodes at the next depth level. It uses a **queue** data structure.

### What is OpenMP?
OpenMP (Open Multi-Processing) is an API that supports **shared-memory multiprocessing** programming in C, C++, and Fortran. It uses compiler directives (`#pragma`) to parallelize code.

### How Parallel BFS Works
1. BFS naturally works **level by level**
2. All nodes at the same level can be processed **in parallel**
3. We use `#pragma omp parallel for` to distribute neighbor exploration across threads
4. `#pragma omp critical` ensures thread-safe access to shared data (visited array, queue)

### Sequential vs Parallel BFS
| Feature | Sequential BFS | Parallel BFS |
|---------|---------------|--------------|
| Execution | Single thread | Multiple threads |
| Speed | Slower for large graphs | Faster for large graphs |
| Complexity | O(V + E) | O((V + E) / p) ideally |
| Overhead | None | Thread creation overhead |

## Algorithm

### Sequential BFS Algorithm:
```
1. Create a visited array, initialize all to false
2. Create a queue, enqueue starting node
3. Mark starting node as visited
4. While queue is not empty:
   a. Dequeue a node
   b. Print the node
   c. For each unvisited neighbor:
      - Mark as visited
      - Enqueue the neighbor
```

### Parallel BFS Algorithm:
```
1. Create a visited array, initialize all to false
2. Create a queue, enqueue starting node
3. Mark starting node as visited
4. While queue is not empty:
   a. Collect all nodes at the current level
   b. Print current level nodes
   c. FOR EACH node at current level IN PARALLEL:
      - For each neighbor:
        - CRITICAL SECTION: if not visited, mark visited and enqueue
```

## Code Explanation

### Key OpenMP Directives Used:
1. **`#pragma omp parallel for`** - Distributes loop iterations across threads
2. **`#pragma omp critical`** - Ensures only one thread executes the block at a time (prevents race conditions)

### Step-by-Step Logic:
1. **Graph Creation**: We create an adjacency list representation of the graph
2. **Sequential BFS**: Standard BFS using a queue - processes one node at a time
3. **Parallel BFS**: 
   - Collects all nodes at current BFS level
   - Distributes neighbor exploration across threads using `#pragma omp parallel for`
   - Uses `#pragma omp critical` to safely update the visited array and queue
4. **Timing**: Uses `omp_get_wtime()` to measure execution time of both approaches

## Dev C++ Setup Instructions

1. Open **Dev C++**
2. Go to **TOOLS → Compiler Options**
3. Go to **Settings → Code Generation**
4. Add: `-fopenmp`
5. Click **OK**
6. Open `bfs_openmp.cpp`
7. Press **F11** to Compile and Run

## Expected Output
```
========================================
 Parallel BFS using OpenMP
========================================
Enter number of vertices: 7
Enter number of edges: 6
Enter edges (u v):
0 1
0 2
1 3
1 4
2 5
2 6
Enter starting node for BFS: 0

Sequential BFS Traversal: 0 1 2 3 4 5 6
Sequential BFS Time: 0.001 ms

Parallel BFS Traversal: 0 1 2 3 4 5 6
Parallel BFS Time: 0.015 ms

========================================
 BFS Completed Successfully!
========================================
```

> **Note**: For small graphs, parallel BFS may be slower due to thread creation overhead. The benefit is visible with large graphs (1000+ nodes).

## Advantages
1. Faster traversal for large graphs
2. Utilizes multiple CPU cores
3. Level-by-level parallelism is natural for BFS
4. Easy to implement with OpenMP directives

## Applications
1. Social network analysis (finding shortest connections)
2. Web crawling
3. GPS navigation systems
4. Network broadcasting
5. Finding shortest path in unweighted graphs

## Time Complexity
| Algorithm | Time Complexity |
|-----------|----------------|
| Sequential BFS | O(V + E) |
| Parallel BFS | O((V + E) / p + D) where p = threads, D = diameter |

## Viva Questions and Answers

**Q1: What is BFS?**
A: BFS (Breadth First Search) is a graph traversal algorithm that explores all vertices at the current depth before moving to the next level. It uses a queue data structure.

**Q2: What data structure does BFS use?**
A: BFS uses a Queue (FIFO - First In First Out).

**Q3: What is OpenMP?**
A: OpenMP is an API for shared-memory parallel programming in C/C++/Fortran. It uses compiler directives like `#pragma omp`.

**Q4: What is `#pragma omp parallel for`?**
A: It distributes loop iterations across multiple threads for parallel execution.

**Q5: What is `#pragma omp critical`?**
A: It ensures that only one thread can execute the enclosed block at a time, preventing race conditions.

**Q6: Why is `#pragma omp critical` needed in parallel BFS?**
A: Because multiple threads may try to update the `visited` array and queue simultaneously. Without critical, this causes data races (incorrect results).

**Q7: What is a race condition?**
A: A race condition occurs when multiple threads access shared data simultaneously and at least one modifies it, leading to unpredictable results.

**Q8: What is the time complexity of BFS?**
A: O(V + E) where V = vertices, E = edges.

**Q9: When is parallel BFS beneficial?**
A: For large graphs with many nodes and edges. For small graphs, the thread overhead may make it slower.

**Q10: What is the difference between BFS and DFS?**
A: BFS uses a queue and explores level by level. DFS uses a stack and explores depth-first. BFS finds shortest path in unweighted graphs.

## Conclusion
We successfully implemented Parallel BFS using OpenMP. The parallel version distributes neighbor exploration across multiple threads, potentially improving performance for large graphs. OpenMP directives like `#pragma omp parallel for` and `#pragma omp critical` make parallelization straightforward.
