// matrix_mul.cu
#include <stdio.h>

__global__ void matMul(float *A, float *B, float *C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < N && col < N) {
        float sum = 0;
        for (int k = 0; k < N; k++)
            sum += A[row * N + k] * B[k * N + col];
        C[row * N + col] = sum;
    }
}

int main() {
    int N = 2;  // 2x2 matrix for simplicity
    size_t size = N * N * sizeof(float);
    float A[] = {1, 2, 3, 4};
    float B[] = {5, 6, 7, 8};
    float C[4];

    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((N+15)/16, (N+15)/16);
    matMul<<<blocks, threads>>>(d_A, d_B, d_C, N);

    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    printf("Result Matrix C:\n");
    for (int i = 0; i < N*N; i++) {
        printf("%f ", C[i]);
        if ((i+1) % N == 0) printf("\n");
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return 0;
}