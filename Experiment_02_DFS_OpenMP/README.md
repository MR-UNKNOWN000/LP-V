# Experiment 02: Parallel Depth First Search (DFS) using OpenMP

## Aim
Design and implement Parallel Depth First Search based on existing algorithms using OpenMP. Use a Tree or an undirected graph for DFS.

## Objective
- Understand DFS algorithm and its depth-first traversal nature
- Learn OpenMP directives: `#pragma omp parallel for` and `#pragma omp critical`
- Implement thread-safe stack-based graph traversal
- Compare sequential vs parallel performance and understand parallelism limitations
- Recognize the inherent sequential nature of DFS and where parallelism can be applied

## Theory

### What is DFS?
Depth First Search (DFS) is a graph traversal algorithm that explores as far as possible along each branch before backtracking. It uses a **stack** data structure (or recursion).

### How Parallel DFS Works
1. DFS explores depth-wise — one path at a time
2. Parallelism is applied when exploring **multiple neighbors** of a node
3. `#pragma omp parallel for` distributes neighbor exploration across threads
4. `#pragma omp critical` ensures thread-safe stack and visited array access

### DFS vs BFS
| Feature | DFS | BFS |
|---------|-----|-----|
| Data Structure | Stack | Queue |
| Traversal | Depth-first | Level-by-level |
| Memory | Less (for sparse graphs) | More (stores entire level) |
| Shortest Path | No (for unweighted) | Yes (for unweighted) |
| Parallelism | Harder to parallelize | Easier (level parallelism) |

### Sequential vs Parallel DFS
| Feature | Sequential DFS | Parallel DFS |
|---------|---------------|--------------|
| Execution | Single thread | Multiple threads |
| Speed | Slower for large graphs | Faster for large graphs |
| Complexity | O(V + E) | Varies with parallelism |
| Overhead | None | Thread creation overhead |

## Algorithm

### Sequential DFS Algorithm:
```
1. Create a visited array, initialize all to false
2. Create a stack, push starting node
3. While stack is not empty:
   a. Pop a node from stack
   b. If not visited:
      - Mark as visited, print it
      - Push all unvisited neighbors onto stack
```

### Parallel DFS Algorithm:
```
1. Create a visited array, initialize all to false
2. Create a stack, push starting node
3. While stack is not empty:
   a. CRITICAL: Pop a node from stack
   b. If not visited:
      - CRITICAL: Mark visited, print it
      - FOR EACH neighbor IN PARALLEL:
        - CRITICAL: If not visited, push to stack
```

## Code Explanation

### Key OpenMP Directives Used:
1. **`#pragma omp parallel for`** - Parallelizes the neighbor exploration loop
2. **`#pragma omp critical`** - Protects shared data (stack, visited array) from race conditions

### Step-by-Step Logic:
1. **Graph Creation**: Adjacency list representation
2. **Sequential DFS**: Uses an iterative approach with explicit stack
3. **Parallel DFS**: 
   - Each neighbor exploration is distributed across threads
   - Critical sections protect shared resources
4. **Timing**: `omp_get_wtime()` measures execution time

## Dev C++ Setup Instructions
1. Open **Dev C++**
2. Go to **TOOLS → Compiler Options**
3. Go to **Settings → Code Generation**
4. Add: `-fopenmp`
5. Click **OK**
6. Open `dfs_openmp.cpp`
7. Press **F11** to Compile and Run

## Expected Output
```
========================================
 Parallel DFS using OpenMP
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
Enter starting node for DFS: 0

Sequential DFS Traversal: 0 1 3 4 2 5 6
Sequential DFS Time: 0.001 ms

Parallel DFS Traversal: 0 1 3 4 2 5 6
Parallel DFS Time: 0.012 ms

========================================
 DFS Completed Successfully!
========================================
```

> **Note**: Parallel DFS output order may vary because threads execute in different orders. The traversal is still correct.

## Advantages
1. Explores deep paths quickly
2. Uses less memory than BFS for sparse graphs
3. Parallelism speeds up neighbor exploration
4. Simple to implement with OpenMP

## Applications
1. Topological sorting
2. Cycle detection in graphs
3. Maze solving
4. Finding connected components
5. Path finding in games

## Time Complexity
| Algorithm | Time Complexity |
|-----------|----------------|
| Sequential DFS | O(V + E) |
| Parallel DFS | O((V + E) / p) ideally |

## Viva Questions and Answers

**Q1: What is DFS?**
A: DFS (Depth First Search) is a graph traversal algorithm that explores as deep as possible along each branch before backtracking. It uses a stack.

**Q2: What data structure does DFS use?**
A: DFS uses a Stack (LIFO - Last In First Out) or recursion.

**Q3: Why is DFS harder to parallelize than BFS?**
A: DFS is inherently sequential (depth-first order). BFS has natural level-based parallelism. DFS parallelism is limited to exploring neighbors simultaneously.

**Q4: What is the difference between recursive and iterative DFS?**
A: Recursive DFS uses the call stack. Iterative DFS uses an explicit stack. Both produce the same traversal. Iterative avoids stack overflow for deep graphs.

**Q5: What is `#pragma omp critical`?**
A: It creates a mutual exclusion zone where only one thread can execute at a time. Used to prevent race conditions on shared data.

**Q6: Can parallel DFS give different traversal orders?**
A: Yes. Since threads may push/pop in different orders, the traversal order may vary between runs. All orders are valid DFS traversals.

**Q7: What is the time complexity of DFS?**
A: O(V + E) where V = vertices and E = edges.

**Q8: When would you use DFS over BFS?**
A: When you need to explore deep paths (maze solving), detect cycles, perform topological sort, or when memory is limited.

**Q9: What is a stack overflow in recursive DFS?**
A: If the graph is very deep, recursive DFS may exceed the maximum call stack size, causing a crash. Iterative DFS avoids this.

**Q10: What is backtracking in DFS?**
A: Backtracking is when DFS returns to a previous node after fully exploring a path. It "backtracks" to explore other unvisited branches.

## Conclusion
We successfully implemented Parallel DFS using OpenMP. The parallel version distributes neighbor exploration across threads. While DFS is inherently harder to parallelize than BFS, OpenMP allows us to achieve some level of parallelism in neighbor exploration.
