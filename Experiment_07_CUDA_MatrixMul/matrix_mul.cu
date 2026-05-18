#include <iostream>
#include <cuda_runtime.h>

using namespace std;

__global__ void multiply(int* a, int* b, int* c, int n)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n)
    {
        int sum = 0;

        for (int k = 0; k < n; k++)
        {
            sum += a[row * n + k] * b[k * n + col];
        }

        c[row * n + col] = sum;
    }
}

void printMatrix(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << a[i * n + j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int n;

    cout << "Enter size of matrix: ";
    cin >> n;

    int* a = new int[n * n];
    int* b = new int[n * n];
    int* c = new int[n * n];

    int size = n * n * sizeof(int);

    cout << "Enter first matrix:" << endl;
    for (int i = 0; i < n * n; i++)
    {
        cin >> a[i];
    }

    cout << "Enter second matrix:" << endl;
    for (int i = 0; i < n * n; i++)
    {
        cin >> b[i];
    }

    int *d_a, *d_b, *d_c;

    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((n + 15) / 16, (n + 15) / 16);

    multiply<<<blocks, threads>>>(d_a, d_b, d_c, n);

    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    cout << "\nResult:" << endl;
    printMatrix(c, n);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}