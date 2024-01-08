// nvcc inner_product_gpu.cu -o inner_product_gpu -O3 -dopt=on
// ./inner_product_gpu

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <iostream>

const int N = 2048*10;

const int nBlockNum = 16;//Open 32 blocks
const int threadnum = 32;//Open 32 threads


/* Three, single-block low-thread reduction vector inner product*/

/* Four, multi-block multi-thread vector inner product */
template <typename T>
__global__ void dot_gpu_3(T *a, T *b, T *c, int n)
{
    __shared__ T aTmp[threadnum];
    const int nThreadIdX = threadIdx.x; //Thread ID index number
    const int nStep = gridDim.x * blockDim.x; // The step length of the step, that is, the number of all threads
    int nTidIdx = blockIdx.x * blockDim.x + threadIdx.x; // The index of the current thread in the global thread

    double dTemp = 0.0;
    while (nTidIdx < n)
    {
        dTemp += a[nTidIdx] * b[nTidIdx];
        nTidIdx += nStep;
    }
    aTmp[nThreadIdX] = dTemp; // Put the inner product in each thread into the shared memory of the corresponding block
    __syncthreads(); // Synchronous operation, that is, wait for the above operations in all threads to be executed

    int i = threadnum / 2;
    while (i != 0)
    {
        if(nThreadIdX < i)
        {
            aTmp[nThreadIdX] += aTmp[nThreadIdX + i];
        }
        __syncthreads(); // Synchronous operation, that is, wait for the above operations in all threads to be executed
        i /= 2;
    }

    if (0 == nThreadIdX)
    {
        c[blockIdx.x] = aTmp[0];
    }

}

int main()
{
    float a[N], b[N];    
    for(int i=0; i<N; ++i) // Assign values ​​to arrays a and b
    {
        a[i] = i * 0.1;
        if(fabs(a[i] < 0.000001))
            b[i] = 0.0;
        else
            b[i] = 1/a[i];
    }

    float *d_a = NULL, *d_b = NULL, *d_c = NULL;
    cudaMalloc(&d_a, N *sizeof(float));
    cudaMemcpy(d_a, a, N *sizeof(float), cudaMemcpyHostToDevice);

    cudaMalloc(&d_b, N *sizeof(float));
    cudaMemcpy(d_b, b, N *sizeof(float), cudaMemcpyHostToDevice);

    cudaMalloc(&d_c, sizeof(float) * nBlockNum);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);
    dot_gpu_3<<< nBlockNum, threadnum >>>(d_a, d_b, d_c, N);
    
    float c_tmp[nBlockNum];
    cudaMemcpy(&c_tmp, d_c, nBlockNum * sizeof(float), cudaMemcpyDeviceToHost);
    
    float c = 0;
    for (int i=0; i < nBlockNum; ++i)
    {
        c += c_tmp[i];
    }
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    std::cout << "Elapsed " << milliseconds << " ms." << std::endl;

    std::cout << c << std::endl;
    printf("--- End of CUDA Inner Productst---\n");
    return 0;
}

