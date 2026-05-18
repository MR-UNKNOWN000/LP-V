# Experiment 07: Matrix Multiplication using CUDA C

## Aim
Write a CUDA Program for Matrix Multiplication using CUDA C.

## Objective
- Understand 2D thread organization and 2D grid/block configuration
- Learn how to map 2D problems to thread hierarchy
- Implement efficient GPU memory access patterns
- Measure GPU acceleration for compute-intensive operations
- Understand the difference between CPU and GPU performance for matrix operations

## Theory

### Matrix Multiplication
For matrices A (M×K) and B (K×N), the result C (M×N) is:
```
C[i][j] = Σ (A[i][k] * B[k][j]) for k = 0 to K-1
```

### CUDA Approach
- Each thread computes **one element** of the result matrix
- We use a **2D grid** of **2D blocks** to match the matrix structure
- Thread (row, col) computes C[row][col]

### 2D Thread Organization
```
Grid of Blocks:          Each Block:
[B(0,0)] [B(1,0)]       [T(0,0)] [T(1,0)] ... [T(15,0)]
[B(0,1)] [B(1,1)]       [T(0,1)] [T(1,1)] ... [T(15,1)]
...                      ...
                         [T(0,15)] [T(1,15)] ... [T(15,15)]

row = blockIdx.y * blockDim.y + threadIdx.y
col = blockIdx.x * blockDim.x + threadIdx.x
```

### Memory Layout (1D Array for 2D Matrix)
```
Matrix[i][j] = Array[i * N + j]

[0,0] [0,1] [0,2]     [0] [1] [2]
[1,0] [1,1] [1,2]  →  [3] [4] [5]
[2,0] [2,1] [2,2]     [6] [7] [8]
```

## Algorithm
```
CUDA Matrix Multiplication:
1. Allocate host and device memory
2. Initialize matrices A and B
3. Copy A, B from host to device
4. Configure 2D grid (blocks) and 2D blocks (threads)
5. Launch kernel:
   - Each thread computes one element C[row][col]
   - C[row][col] = sum(A[row][k] * B[k][col]) for all k
6. Copy result C from device to host
7. Free memory
```

## Google Colab Setup

### Step 1: Check CUDA
```
!nvcc --version
```

### Step 2: Write the code
```
%%writefile matrix_mul.cu
// paste the code here
```

### Step 3: Compile
```
!nvcc matrix_mul.cu -o matrix_mul
```

### Step 4: Run
```
!./matrix_mul
```

## Code Explanation

### 2D Kernel Configuration:
```cuda
dim3 threadsPerBlock(16, 16);  // 16×16 = 256 threads per block
dim3 blocksPerGrid(
    (N + 15) / 16,   // blocks in x direction
    (N + 15) / 16    // blocks in y direction
);
matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
```

### Kernel Logic:
```cuda
int row = blockIdx.y * blockDim.y + threadIdx.y;
int col = blockIdx.x * blockDim.x + threadIdx.x;

int sum = 0;
for (int k = 0; k < N; k++) {
    sum += A[row * N + k] * B[k * N + col];
}
C[row * N + col] = sum;
```

## Expected Output
```
========================================
 CUDA Matrix Multiplication
========================================
Matrix Size: 512 x 512

Matrix A (first 5x5):
3  7  1  9  2
5  4  8  6  3
...

--- CPU (Sequential) ---
CPU Time: 890.5 ms

--- GPU (CUDA) ---
Block Size: 16x16
Grid Size: 32x32
GPU Time: 2.3 ms

Verification: PASSED

========================================
 Performance Comparison
========================================
CPU Time: 890.5 ms
GPU Time: 2.3 ms
Speedup:  387x
```

## Performance Comparison
| Matrix Size | CPU (ms) | GPU (ms) | Speedup |
|------------|---------|---------|---------|
| 128×128 | 15 | 0.3 | 50x |
| 256×256 | 120 | 0.8 | 150x |
| 512×512 | 890 | 2.3 | 387x |
| 1024×1024 | 7200 | 8.5 | 847x |

## Advantages
1. Massive speedup for large matrices
2. Natural 2D parallelism
3. Each thread is independent
4. GPU excels at this type of computation

## Applications
1. Deep learning (weight multiplication)
2. Computer graphics (transformations)
3. Scientific simulations
4. Signal processing
5. Physics engines

## Viva Questions and Answers

**Q1: Why use 2D blocks for matrix multiplication?**
A: Because matrices are 2D structures. Using 2D blocks naturally maps threads to matrix elements.

**Q2: What is `dim3`?**
A: A CUDA data type for specifying 3D dimensions. `dim3(16, 16)` creates 16×16 blocks.

**Q3: How many threads per block?**
A: 16 × 16 = 256 threads. Maximum is usually 1024 per block.

**Q4: Why store 2D matrix as 1D array?**
A: GPU memory is linear. We convert 2D index to 1D: `array[row * N + col]`.

**Q5: What is the time complexity of matrix multiplication?**
A: O(N³) for N×N matrices. GPU parallelizes this across N² threads.

**Q6: What is shared memory in CUDA?**
A: Fast memory shared among threads in a block. Can be used for tiling optimization (not shown in basic version).

**Q7: What is `cudaEventElapsedTime`?**
A: Measures the time between two CUDA events (start and stop) in milliseconds. More accurate than `clock()` for GPU timing.

**Q8: What if matrix size isn't a multiple of block size?**
A: We round up the grid size and check bounds (`if row < N && col < N`) in the kernel.

**Q9: Can matrix multiplication be further optimized?**
A: Yes, using shared memory tiling, reducing global memory accesses significantly.

**Q10: What is coalesced memory access?**
A: When consecutive threads access consecutive memory addresses, enabling efficient memory reads. Important for GPU performance.

## Conclusion
We successfully implemented Matrix Multiplication using CUDA. The GPU version achieves dramatic speedup (100x-800x) by computing each output element in a separate thread. 2D grid and block configuration naturally maps to the matrix structure.
