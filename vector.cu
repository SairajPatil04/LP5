// vector_add.cu
#include <stdio.h>

__global__ void vecAdd(float *A, float *B, float *C, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N)
        C[i] = A[i] + B[i];
}

int main() {
    int N = 5;
    size_t size = N * sizeof(float);
    float A[] = {1, 2, 3, 4, 5};
    float B[] = {10, 20, 30, 40, 50};
    float C[5];

    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    vecAdd<<<1, N>>>(d_A, d_B, d_C, N);

    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    printf("Result Vector C:\n");
    for (int i = 0; i < N; i++)
        printf("%f ", C[i]);
    printf("\n");

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return 0;
}