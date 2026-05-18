# Experiment 06: Addition of Two Large Vectors using CUDA

## Aim
Write a CUDA Program for addition of two large vectors.

## Objective
- Understand CUDA programming model: Host, Device, Grid, Block, Thread hierarchy
- Learn GPU memory management: allocation, host-device data transfer, deallocation
- Implement a simple CUDA kernel for parallel vector operations
- Measure GPU vs CPU performance and understand memory transfer overhead
- Understand thread indexing and grid/block configuration

## Theory

### What is CUDA?
CUDA (Compute Unified Device Architecture) is NVIDIA's parallel computing platform. It allows using the GPU for general-purpose computing (GPGPU).

### Key CUDA Concepts
| Concept | Description |
|---------|-------------|
| **Host** | CPU and its memory (RAM) |
| **Device** | GPU and its memory (VRAM) |
| **Kernel** | Function that runs on the GPU |
| **Thread** | Smallest unit of execution on GPU |
| **Block** | Group of threads |
| **Grid** | Group of blocks |
| **`__global__`** | Keyword for kernel functions |

### CUDA Memory Flow
```
1. Allocate GPU memory (cudaMalloc)
2. Copy data CPU → GPU (cudaMemcpy, HostToDevice)
3. Launch kernel on GPU
4. Copy result GPU → CPU (cudaMemcpy, DeviceToHost)
5. Free GPU memory (cudaFree)
```

### Thread Organization for Vector Addition
```
Vector: [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] ...
Thread:  T0  T1  T2  T3  T4  T5  T6  T7  T8  T9 ...

Each thread handles exactly ONE element.
Thread ID = blockIdx.x * blockDim.x + threadIdx.x
```

### CPU vs GPU
| Feature | CPU | GPU |
|---------|-----|-----|
| Cores | 4-16 | Thousands |
| Thread Speed | Fast | Slower individually |
| Best For | Sequential tasks | Massively parallel |
| Memory | Large RAM | Limited VRAM |

## Algorithm
```
1. Allocate host memory for A, B, C
2. Initialize A and B with random values
3. Allocate device memory for d_A, d_B, d_C
4. Copy A → d_A, B → d_B (Host to Device)
5. Launch kernel: each thread computes C[i] = A[i] + B[i]
6. Copy d_C → C (Device to Host)
7. Free all memory
```

## Google Colab Setup

### Step 1: Check CUDA
```
!nvcc --version
```

### Step 2: Write the code
```
%%writefile vector_add.cu
// paste the code here
```

### Step 3: Compile
```
!nvcc vector_add.cu -o vector_add
```

### Step 4: Run
```
!./vector_add
```

## Code Explanation

### CUDA Kernel:
```cuda
__global__ void vectorAdd(int* A, int* B, int* C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        C[i] = A[i] + B[i];
    }
}
```
- `__global__` = function runs on GPU, called from CPU
- `blockIdx.x` = which block this thread belongs to
- `blockDim.x` = number of threads per block (256)
- `threadIdx.x` = thread's index within its block
- `i` = global thread ID = unique index into the array

### Memory Operations:
```cuda
cudaMalloc(&d_A, size);                              // Allocate GPU memory
cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice); // CPU → GPU
cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost); // GPU → CPU
cudaFree(d_A);                                       // Free GPU memory
```

### Kernel Launch:
```cuda
vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);
// blocksPerGrid = (n + 255) / 256
// threadsPerBlock = 256
```

## Expected Output
```
========================================
 CUDA Vector Addition
========================================
Vector Size: 1000000 elements

First 10 elements of A: 234 567 123 ...
First 10 elements of B: 456 789 321 ...

--- CPU (Sequential) ---
First 10 results: 690 1356 444 ...
CPU Time: 3.2 ms

--- GPU (CUDA) ---
Threads per Block: 256
Blocks per Grid: 3907
First 10 results: 690 1356 444 ...
GPU Time: 0.15 ms

Verification: PASSED

========================================
 Performance Comparison
========================================
CPU Time: 3.2 ms
GPU Time: 0.15 ms
Speedup:  21.3x
```

## Advantages
1. Massive parallelism (millions of threads)
2. Significant speedup for large vectors
3. Simple kernel code
4. Scalable to any vector size

## Applications
1. Image processing (pixel operations)
2. Scientific simulations
3. Machine learning (tensor operations)
4. Signal processing
5. Financial modeling

## Viva Questions and Answers

**Q1: What is CUDA?**
A: CUDA is NVIDIA's parallel computing platform that allows using GPUs for general-purpose computing.

**Q2: What is a CUDA kernel?**
A: A function that runs on the GPU, marked with `__global__` keyword. Called from CPU, executed by thousands of GPU threads.

**Q3: What is the difference between Host and Device?**
A: Host = CPU + RAM. Device = GPU + VRAM. Data must be explicitly copied between them.

**Q4: What is `cudaMemcpy`?**
A: Function to copy data between CPU and GPU memory. Uses direction flags: `cudaMemcpyHostToDevice` or `cudaMemcpyDeviceToHost`.

**Q5: How is thread ID calculated?**
A: `globalID = blockIdx.x * blockDim.x + threadIdx.x`

**Q6: Why do we check `if (i < n)` in the kernel?**
A: Because the total number of threads may exceed the array size (blocks are rounded up). We avoid out-of-bounds access.

**Q7: What is `cudaMalloc`?**
A: Allocates memory on the GPU (device memory). Similar to `malloc` for CPU.

**Q8: What is a thread block?**
A: A group of threads that can cooperate and share memory. Typical size: 256 or 512 threads.

**Q9: What is the grid?**
A: The collection of all thread blocks. Grid size = total elements / block size.

**Q10: Why is GPU faster for vector addition?**
A: GPU has thousands of cores processing elements simultaneously, while CPU does one element at a time.

## Conclusion
We successfully implemented vector addition using CUDA. The GPU version achieves significant speedup over CPU by processing millions of elements simultaneously. Key steps include memory allocation, data transfer, kernel launch, and result retrieval.
