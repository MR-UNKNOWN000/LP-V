# Experiment 03: Parallel Bubble Sort using OpenMP

## Aim
Write a program to implement Parallel Bubble Sort using OpenMP. Use existing algorithms and measure the performance of sequential and parallel algorithms.

## Objective
- Understand why regular Bubble Sort cannot be parallelized directly
- Learn the Odd-Even Transposition Sort approach to parallelize sorting
- Implement `#pragma omp parallel for` for non-overlapping data accesses
- Measure performance improvement for various array sizes
- Understand thread creation overhead vs parallelism benefit tradeoff

## Theory

### What is Bubble Sort?
Bubble Sort is a simple sorting algorithm that repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. This process repeats until the list is sorted.

### Odd-Even Transposition Sort (Parallel Bubble Sort)
To parallelize Bubble Sort, we use **Odd-Even Transposition Sort**:
1. **Even Phase**: Compare and swap pairs at even indices: (0,1), (2,3), (4,5), ...
2. **Odd Phase**: Compare and swap pairs at odd indices: (1,2), (3,4), (5,6), ...
3. Since pairs don't overlap, comparisons within each phase can run **in parallel**

### Why Regular Bubble Sort Can't Be Parallelized Directly
In regular Bubble Sort, each comparison depends on the previous swap. Odd-Even Transposition Sort removes this dependency by ensuring non-overlapping pairs.

### Sequential vs Parallel Comparison
| Feature | Sequential Bubble Sort | Parallel Bubble Sort |
|---------|----------------------|---------------------|
| Algorithm | Standard Bubble Sort | Odd-Even Transposition |
| Execution | Single thread | Multiple threads |
| Time Complexity | O(n²) | O(n²/p) ideally |
| Speedup | 1x | Up to px |

## Algorithm

### Sequential Bubble Sort:
```
1. For i = 0 to n-1:
   2. For j = 0 to n-i-1:
      3. If arr[j] > arr[j+1]:
         4. Swap arr[j] and arr[j+1]
```

### Parallel Bubble Sort (Odd-Even Transposition):
```
1. For phase = 0 to n-1:
   2. If phase is EVEN:
      3. FOR i = 0, 2, 4, ... IN PARALLEL:
         - If arr[i] > arr[i+1]: Swap
   4. If phase is ODD:
      5. FOR i = 1, 3, 5, ... IN PARALLEL:
         - If arr[i] > arr[i+1]: Swap
```

## Code Explanation

### Key OpenMP Directive:
- **`#pragma omp parallel for`** - Distributes the comparison-swap operations across threads
- No critical section needed because pairs don't overlap!

### Step-by-Step Logic:
1. Generate a random array
2. Copy it for fair comparison
3. Sort copy 1 with Sequential Bubble Sort (time it)
4. Sort copy 2 with Parallel Odd-Even Sort (time it)
5. Compare performance

## Dev C++ Setup Instructions
1. Open **Dev C++**
2. Go to **TOOLS → Compiler Options**
3. Go to **Settings → Code Generation**
4. Add: `-fopenmp`
5. Click **OK**
6. Open `bubblesort_openmp.cpp`
7. Press **F11** to Compile and Run

## Expected Output
```
========================================
 Parallel Bubble Sort using OpenMP
========================================
Enter number of elements: 10000

Original Array: 4521 7832 1234 ...

Sequential Sorted Array: 1 2 3 4 ...
Sequential Bubble Sort Time: 245.32 ms

Parallel Sorted Array: 1 2 3 4 ...
Parallel Bubble Sort Time: 98.45 ms

========================================
 Performance Comparison
========================================
Sequential Time: 245.32 ms
Parallel Time:   98.45 ms
Speedup:         2.49x
========================================
```

## Performance Comparison

| Array Size | Sequential (ms) | Parallel (ms) | Speedup |
|-----------|----------------|---------------|---------|
| 1,000 | ~5 | ~8 | 0.6x (overhead) |
| 10,000 | ~250 | ~100 | 2.5x |
| 50,000 | ~6,000 | ~2,000 | 3.0x |

> **Note**: Parallel version shows benefit only for larger arrays (5000+).

## Advantages
1. Simple to understand and implement
2. Good for teaching parallel concepts
3. Non-overlapping pairs allow safe parallelism
4. No critical sections needed in odd-even approach

## Applications
1. Educational purposes (learning parallelism)
2. Small dataset sorting
3. Nearly sorted data (bubble sort is efficient)
4. Embedded systems with limited resources

## Time Complexity
| Algorithm | Best Case | Average Case | Worst Case |
|-----------|-----------|-------------|------------|
| Sequential Bubble Sort | O(n) | O(n²) | O(n²) |
| Parallel Bubble Sort | O(n) | O(n²/p) | O(n²/p) |

## Viva Questions and Answers

**Q1: What is Bubble Sort?**
A: Bubble Sort repeatedly compares adjacent elements and swaps them if they're in the wrong order. The largest element "bubbles" to the end in each pass.

**Q2: What is Odd-Even Transposition Sort?**
A: It's a parallel version of Bubble Sort where even-indexed pairs are compared in one phase and odd-indexed pairs in the next. Non-overlapping pairs allow parallel execution.

**Q3: Why can't regular Bubble Sort be parallelized directly?**
A: Because each comparison depends on the previous swap result. Adjacent comparisons share elements, creating data dependencies.

**Q4: Why don't we need `#pragma omp critical` in Odd-Even Sort?**
A: Because pairs (0,1), (2,3), (4,5) don't share any elements, so there's no data race. Each thread works on independent data.

**Q5: What is the time complexity of Bubble Sort?**
A: O(n²) in average and worst case, O(n) in best case (already sorted).

**Q6: What is speedup?**
A: Speedup = Sequential Time / Parallel Time. It measures how much faster the parallel version is.

**Q7: Why might parallel Bubble Sort be slower for small arrays?**
A: Thread creation and synchronization overhead exceeds the benefit of parallelism for small data.

**Q8: Is Bubble Sort stable?**
A: Yes, Bubble Sort is a stable sorting algorithm. Equal elements maintain their relative order.

**Q9: What is `#pragma omp parallel for`?**
A: It distributes loop iterations across multiple threads for parallel execution.

**Q10: Can memory be optimized in Bubble Sort?**
A: Yes, use in-place sorting to avoid temporary arrays. Odd-Even implementation already does this.

## Conclusion

**Q10: What is the best sorting algorithm?**
A: It depends on the use case. Quick Sort and Merge Sort (O(n log n)) are generally faster. Bubble Sort is mainly used for educational purposes.

## Conclusion
We successfully implemented Parallel Bubble Sort using the Odd-Even Transposition Sort technique with OpenMP. The parallel version shows significant speedup for large arrays due to independent pair comparisons that can run simultaneously across multiple threads.
