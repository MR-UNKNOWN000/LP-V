#include <iostream>
#include <cuda_runtime.h>

using namespace std;

__global__ void add(int* a, int* b, int* c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    int* a = new int[n];
    int* b = new int[n];
    int* c = new int[n];

    cout << "Enter first vector: ";
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    cout << "Enter second vector: ";
    for (int i = 0; i < n; i++)
    {
        cin >> b[i];
    }

    int *d_a, *d_b, *d_c;
    int size = n * sizeof(int);

    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    add<<<blocks, threads>>>(d_a, d_b, d_c, n);

    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    cout << "\nResult: ";
    for (int i = 0; i < n; i++)
    {
        cout << c[i] << " ";
    }
    cout << endl;

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}