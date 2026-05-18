# Experiment 05: Parallel Reduction - Min, Max, Sum, Average using OpenMP

## Aim
Implement Min, Max, Sum, and Average operations using Parallel Reduction with OpenMP.

## Objective
- Understand the reduction operation and its importance in parallel computing
- Learn OpenMP `reduction` clause syntax and supported operations
- Implement parallel reduction for multiple operations (min, max, sum, average)
- Compare sequential vs parallel reduction performance
- Recognize why reduction is preferred over critical sections for aggregation

## Theory

### What is Parallel Reduction?
Parallel Reduction is a technique where multiple elements are combined into a single result using an associative operation (like min, max, sum). Instead of processing sequentially, multiple threads compute partial results, which are then combined.

### How OpenMP Reduction Works
1. Each thread gets a **private copy** of the reduction variable
2. Each thread computes its **partial result** on its chunk of data
3. At the end, OpenMP **combines** all partial results using the specified operator
4. This is done automatically with `#pragma omp parallel for reduction(op: var)`

### Reduction Operations
| Operation | OpenMP Syntax | Initial Value |
|-----------|--------------|---------------|
| Sum | `reduction(+: sum)` | 0 |
| Min | `reduction(min: min_val)` | First element |
| Max | `reduction(max: max_val)` | First element |

### Sequential vs Parallel
| Feature | Sequential | Parallel Reduction |
|---------|-----------|-------------------|
| Threads | 1 | Multiple |
| Time | O(n) | O(n/p + log p) |
| Approach | Linear scan | Divide and combine |

## Algorithm
```
PARALLEL REDUCTION for SUM:
1. Divide array into chunks (one per thread)
2. Each thread computes partial sum of its chunk
3. Combine all partial sums into final result

PARALLEL REDUCTION for MIN:
1. Each thread finds minimum of its chunk
2. Combine all partial minimums using min()

PARALLEL REDUCTION for MAX:
1. Each thread finds maximum of its chunk
2. Combine all partial maximums using max()

AVERAGE = SUM / n
```

## Code Explanation

### Key OpenMP Directive:
```cpp
#pragma omp parallel for reduction(+: sum)
// Each thread gets private 'sum', adds its portion, then all are combined
```

### The `reduction` Clause:
- **`reduction(+: sum)`** - Addition reduction
- **`reduction(min: min_val)`** - Minimum reduction
- **`reduction(max: max_val)`** - Maximum reduction

## Dev C++ Setup Instructions
1. Open **Dev C++**
2. Go to **TOOLS → Compiler Options**
3. Go to **Settings → Code Generation**
4. Add: `-fopenmp`
5. Click **OK**
6. Press **F11** to Compile and Run

## Expected Output
```
========================================
 Parallel Reduction: Min, Max, Sum, Avg
========================================
Enter number of elements: 1000000

--- Sequential Operations ---
Min = 0 (Time: 2.5 ms)
Max = 99999 (Time: 2.3 ms)
Sum = 49993245678 (Time: 2.1 ms)
Average = 49993.2 (Time: 2.1 ms)
Total Sequential Time: 9.0 ms

--- Parallel Operations ---
Min = 0 (Time: 0.8 ms)
Max = 99999 (Time: 0.7 ms)
Sum = 49993245678 (Time: 0.6 ms)
Average = 49993.2 (Time: 0.6 ms)
Total Parallel Time: 2.7 ms

========================================
 Performance Comparison
========================================
Total Sequential Time: 9.0 ms
Total Parallel Time:   2.7 ms
Speedup:               3.33x

--- Verification ---
Min Match: YES
Max Match: YES
Sum Match: YES
Avg Match: YES
```

## Advantages
1. Automatic thread management with OpenMP reduction
2. No critical sections needed (OpenMP handles it)
3. Scales well with number of cores
4. Simple syntax for complex parallel operations

## Applications
1. Statistical analysis (mean, variance)
2. Image processing (brightness, contrast)
3. Financial calculations (portfolio analysis)
4. Scientific computing (vector norms)
5. Database aggregations

## Time Complexity
| Operation | Sequential | Parallel |
|-----------|-----------|----------|
| Min/Max/Sum | O(n) | O(n/p + log p) |
| Average | O(n) | O(n/p + log p) |

## Viva Questions and Answers

**Q1: What is parallel reduction?**
A: It's a technique to combine array elements into a single value using multiple threads, where each thread handles a portion and results are combined.

**Q2: What is the `reduction` clause in OpenMP?**
A: It creates private copies for each thread, computes partial results, and automatically combines them using the specified operator (+, min, max, etc.).

**Q3: What operators can be used with reduction?**
A: +, -, *, &, |, ^, &&, ||, min, max.

**Q4: Why is reduction better than using critical sections?**
A: Reduction avoids the bottleneck of critical sections. Each thread works independently on private copies, and combining happens efficiently at the end.

**Q5: What is the time complexity of parallel reduction?**
A: O(n/p + log p) where n = elements, p = threads. The log p comes from combining partial results.

**Q6: Can reduction be used for non-associative operations?**
A: No. Reduction requires associative operations (where order doesn't matter: a+b = b+a).

**Q7: What happens if we don't use reduction for sum?**
A: We'd need `#pragma omp critical` around the sum update, which creates a bottleneck.

**Q8: What is the initial value for + reduction?**
A: 0 (identity element for addition). For * it's 1, for min it's INT_MAX.

**Q9: How many threads does OpenMP use by default?**
A: Usually the number of CPU cores. Can be set with `omp_set_num_threads()` or `OMP_NUM_THREADS` environment variable.

**Q10: What is a private variable in OpenMP?**
A: Each thread gets its own copy. Changes to the private copy don't affect the original or other threads' copies.

## Conclusion
We successfully implemented Min, Max, Sum, and Average operations using OpenMP's parallel reduction. The `reduction` clause simplifies parallelization by automatically handling private copies and result combination. Significant speedup is observed for large arrays.
