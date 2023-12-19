#include <cuda.h>
#include <iostream>

__global__ void cuda_hello()
{
    printf("Hello World from GPU!\n");
}

int main ()
{
    cuda_hello<<<1,1>>>();

    int deviceCount;
    cudaError_t e = cudaGetDeviceCount(&deviceCount);
    printf("deviceCount = %d", deviceCount);
    return e == cudaSuccess ? deviceCount : -1;
}