// Вычисление скалярного произведения на GPU
// nvcc vecMultGpu.cu -o app
// ./app

#include "vector1d.c"


#include <stdio.h>

cudaError_t vecMultCuda(const vector1d *v1, const vector1d *v2, double* result);

__global__ void vecMultKernel(vector1d *v1, vector1d *v2, double* result)
{
    int i = threadIdx.x;
    //c[i] = a[i] + b[i];
    printf("GPU v1: Length = %d\n", v1->length);
    printf("GPU v1: Data[0] = %lf\n", v1->data[0]);
    printf("GPU v2: Length = %d\n", v2->length);
    printf("GPU v2: Data[0] = %lf\n", v2->data[0]);

    double res = 0;
    for(size_t i = 0; i < v1->length; i++)
        res += v1->data[i] * v2->data[i];

    *result = res;
}

int main()
{
    size_t size = 10;
    
    vector1d *v1 = vector1d_create(size); 
    vector1d *v2 = vector1d_create(size);

    double k = 0.5;
    int min = 10;
    int max = 20;


    clock_t start, end;
    double cpu_time_used;
        
    vector1d_init_scalar(v1, v2, k, min, max);
    vector1d_print(v1, "v1 = ");
    vector1d_print(v2, "v2 = ");
    
    start = clock();

    // Вычисление скалярного произведения
    double scalar_mult_result = 0;
    cudaError_t cudaStatus = vecMultCuda(v1, v2, &scalar_mult_result);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "vecMultCuda failed!");
        return 1;
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cpu_time_used = %lf sec\n", cpu_time_used);

    printf("scalar_mult_result = %lf", scalar_mult_result);


    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

// Helper function for using CUDA to multiply vectors in parallel.
cudaError_t vecMultCuda(const vector1d *v1, const vector1d *v2, double* result)
{
    vector1d *dev_v1 = 0;
    vector1d *dev_v2 = 0;
    double *dev_result = 0;
    cudaError_t cudaStatus;

    // Выбор GPU для запуска, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)
    double *dev_v1_data = 0;
    cudaStatus = cudaMalloc((void**)&dev_v1_data, v1->length * sizeof(double));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }    
    cudaStatus = cudaMalloc((void**)&dev_v1, sizeof(vector1d));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    vector1d v1_dto;
    v1_dto.length = v1->length;
    v1_dto.data = dev_v1_data;


    double *dev_v2_data = 0;
    cudaStatus = cudaMalloc((void**)&dev_v2_data, v2->length * sizeof(double));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    cudaStatus = cudaMalloc((void**)&dev_v2, sizeof(vector1d));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    vector1d v2_dto;
    v2_dto.length = v2->length;
    v2_dto.data = dev_v2_data;

    cudaStatus = cudaMalloc((void**)&dev_result, sizeof(double));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_v1_data, v1->data, v1->length * sizeof(double), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    cudaStatus = cudaMemcpy(dev_v1, &v1_dto, sizeof(vector1d), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_v2_data, v2->data, v2->length * sizeof(double), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    cudaStatus = cudaMemcpy(dev_v2, &v2_dto, sizeof(vector1d), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    vecMultKernel<<<1, 1>>>(dev_v1, dev_v2, dev_result);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "vecMultKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(result, dev_result, sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    } // */

Error:
    cudaFree(dev_v1);
    cudaFree(dev_v2);
    cudaFree(dev_result);
    
    return cudaStatus;
}
