# Experiment 04: Parallel Merge Sort using OpenMP

## Aim
Write a program to implement Merge Sort using OpenMP. Use existing algorithms and measure the performance of sequential and parallel algorithms.

## Objective
- Understand divide-and-conquer strategy and natural parallelism in Merge Sort
- Learn `#pragma omp parallel sections` for independent task parallelization
- Implement depth-limiting to prevent excessive thread creation
- Measure performance improvement and understand optimal depth levels
- Compare with sequential merge and identify bottlenecks (merge step)

## Theory

### What is Merge Sort?
Merge Sort is a **divide-and-conquer** sorting algorithm that:
1. **Divides** the array into two halves
2. **Recursively sorts** each half
3. **Merges** the two sorted halves

### How Parallel Merge Sort Works
1. Merge Sort naturally divides into **two independent subproblems**
2. Each half can be sorted by a **separate thread**
3. We use `#pragma omp parallel sections` to sort both halves simultaneously
4. The merge step remains sequential (both halves must be sorted first)

### Sequential vs Parallel Comparison
| Feature | Sequential Merge Sort | Parallel Merge Sort |
|---------|---------------------|-------------------|
| Execution | Single thread | Multiple threads |
| Divide | Sequential | Parallel (sections) |
| Merge | Sequential | Sequential |
| Time Complexity | O(n log n) | O(n log n / p) |

## Algorithm

### Sequential Merge Sort:
```
mergeSort(arr, left, right):
  1. If left < right:
     2. mid = (left + right) / 2
     3. mergeSort(arr, left, mid)
     4. mergeSort(arr, mid+1, right)
     5. merge(arr, left, mid, right)
```

### Parallel Merge Sort:
```
mergeSort_parallel(arr, left, right, depth):
  1. If left < right:
     2. mid = (left + right) / 2
     3. If depth <= 3:
        4. PARALLEL SECTIONS:
           Section 1: mergeSort_parallel(left, mid, depth+1)
           Section 2: mergeSort_parallel(mid+1, right, depth+1)
     5. Else: (sequential for deeper levels)
        6. mergeSort_sequential(left, mid)
        7. mergeSort_sequential(mid+1, right)
     8. merge(arr, left, mid, right)
```

## Code Explanation

### Key OpenMP Directives:
1. **`#pragma omp parallel sections`** - Creates parallel sections where each section runs on a different thread
2. **`#pragma omp section`** - Defines an individual section within parallel sections

### Depth Limiting:
- We limit parallel depth to 3 levels (creating up to 2³ = 8 threads)
- Beyond depth 3, we switch to sequential sort
- This prevents creating too many threads (which causes overhead)

### Step-by-Step Logic:
1. Generate random array
2. Copy for fair comparison
3. Sort with Sequential Merge Sort (timed)
4. Sort with Parallel Merge Sort (timed)
5. Compare performance

## Dev C++ Setup Instructions
1. Open **Dev C++**
2. Go to **TOOLS → Compiler Options**
3. Go to **Settings → Code Generation**
4. Add: `-fopenmp`
5. Click **OK**
6. Open `mergesort_openmp.cpp`
7. Press **F11** to Compile and Run

## Expected Output
```
========================================
 Parallel Merge Sort using OpenMP
========================================
Enter number of elements: 100000

Original Array: 4521 7832 1234 ...

Sequential Sorted Array: 0 1 2 3 ...
Sequential Merge Sort Time: 45.32 ms

Parallel Sorted Array: 0 1 2 3 ...
Parallel Merge Sort Time: 18.71 ms

========================================
 Performance Comparison
========================================
Sequential Time: 45.32 ms
Parallel Time:   18.71 ms
Speedup:         2.42x
========================================
```

## Performance Comparison

| Array Size | Sequential (ms) | Parallel (ms) | Speedup |
|-----------|----------------|---------------|---------|
| 10,000 | ~5 | ~6 | 0.8x |
| 100,000 | ~45 | ~19 | 2.4x |
| 1,000,000 | ~500 | ~180 | 2.8x |

## Advantages
1. O(n log n) guaranteed time complexity
2. Natural divide-and-conquer parallelism
3. Stable sorting algorithm
4. Depth limiting prevents thread explosion

## Applications
1. External sorting (large files)
2. Linked list sorting
3. Counting inversions
4. Database operations
5. Multi-threaded applications

## Time Complexity
| Algorithm | Best | Average | Worst | Space |
|-----------|------|---------|-------|-------|
| Sequential Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Parallel Merge Sort | O(n log n / p) | O(n log n / p) | O(n log n / p) | O(n) |

## Viva Questions and Answers

**Q1: What is Merge Sort?**
A: Merge Sort is a divide-and-conquer algorithm that divides the array into halves, recursively sorts each half, and merges them back together.

**Q2: What is the time complexity of Merge Sort?**
A: O(n log n) in all cases (best, average, worst).

**Q3: Is Merge Sort stable?**
A: Yes, Merge Sort is stable. Equal elements maintain their relative order.

**Q4: What is `#pragma omp parallel sections`?**
A: It creates a block where different sections can run on different threads simultaneously.

**Q5: Why do we limit the recursion depth for parallelism?**
A: Creating too many threads causes overhead. Limiting depth to 3-4 levels creates 8-16 threads, which is enough for most CPUs.

**Q6: How does Merge Sort achieve parallelism?**
A: The two recursive calls (left half and right half) are independent and can run on separate threads simultaneously.

**Q7: Can the merge step be parallelized?**
A: Yes, but it's complex. Simple parallel merge sorts keep the merge sequential while parallelizing the recursive sorting.

**Q8: What is the space complexity of Merge Sort?**
A: O(n) additional space for the temporary arrays used during merging.

**Q9: How is Merge Sort different from Quick Sort?**
A: Merge Sort divides into equal halves (guaranteed O(n log n)), while Quick Sort partitions around a pivot (O(n²) worst case). Merge Sort needs extra space; Quick Sort is in-place.

**Q10: What is divide-and-conquer?**
A: A strategy that divides a problem into smaller subproblems, solves them independently, and combines the results.

## Conclusion
We successfully implemented Parallel Merge Sort using OpenMP. Using `#pragma omp parallel sections`, both halves of the array are sorted simultaneously, achieving significant speedup for large arrays. Depth limiting ensures optimal thread usage.
