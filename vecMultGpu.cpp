// Вычисление скалярного произведения на GPU
// nvcc vecMultGpu.cpp -o app
// ./app

#include "vector1d.c"
#include <thread>
#include <mutex>
#include <vector>

std::mutex mutex;
//double scalar_mult_result = 0;

// Функция вычисления скалярного произведения
// *v1 - указатель на вектор 1
// *v2 - указатель на вектор 2
// startInd - индекс элемента, с которого будет рассчитывать свой фрагмент текущий поток
// length - количество элементов (размер фрагмента), которое будет рассчитывать текущий поток
// result - ссылка на переменную, куда будет записан результат вычисления скалярного произведения
void scalar_mult_cpu_thread(vector1d *v1, vector1d *v2, size_t startInd, size_t length, double& result)
{
    printf("scalar_mult_cpu_thread\n");
    double temp = 0;
    for(size_t i = startInd; i < startInd+length; i++)
    {
        temp += v1->data[i] * v2->data[i];
    }

    mutex.lock();
    result += temp;
    mutex.unlock();
}

__global__ void cuda_hello()
{
    printf("Hello World from GPU!\n");
}

int main()
{
    cuda_hello<<<1,1>>>();

    size_t size = 10000000;
    
    vector1d *v1 = vector1d_create(size);
    //printf("*v1 = %p\n", v1);
    //printf("v1->length = %d\n", v1->length);

    vector1d *v2 = vector1d_create(size);
    //printf("*v2 = %p\n", v2);    
    //printf("v2->length = %d\n", v2->length);

    double k = 0.5;
    int min = 10;
    int max = 20;

    clock_t start, end;
    double cpu_time_used;
        
    vector1d_init_scalar(v1, v2, k, min, max);
    //vector1d_print(v1, "v1 = ");
    //vector1d_print(v2, "v2 = ");
    
    start = clock();

    double scalar_mult_result = 0;
    int NumCpuThreads = 4;
    std::vector<std::thread> threads;

    if(v1->length % NumCpuThreads == 0)//Если количество узлов делится на количество потоков нацело, размеры данных для каждого потока будут одинаковы
    {
        int NbyThread = v1->length / NumCpuThreads;
        for(size_t i = 0; i < NumCpuThreads; i++)
            threads.push_back(std::thread(scalar_mult_cpu_thread, v1, v2, i*NbyThread, NbyThread, std::ref(scalar_mult_result)));
    }
    else
    {
        int NbyThread = v1->length / (NumCpuThreads-1);
        int NbyThreadLast = v1->length % (NumCpuThreads-1);
        printf("NbyThread = %d, NbyThreadLast = %d\n", NbyThread, NbyThreadLast);

        for(size_t i = 0; i < NumCpuThreads-1; i++)
            threads.push_back(std::thread(scalar_mult_cpu_thread, v1, v2, i*NbyThread, NbyThread, std::ref(scalar_mult_result)));

        threads.push_back(std::thread(scalar_mult_cpu_thread, v1, v2, (NumCpuThreads-1)*NbyThread, NbyThreadLast, std::ref(scalar_mult_result)));
    }

    for(auto& thread : threads)
    {
        if(thread.joinable())
            thread.join();
    }
    

    /*std::thread t1(scalar_mult_cpu_thread, v1, v2, 0, v1->length); // + Вычисление скалярного произведения одном потоком
    t1.join(); // */

    /*std::thread t1(scalar_mult_cpu_thread, v1, v2, 0,            v1->length/2); // + Вычисление скалярного произведения двум потоками
    std::thread t2(scalar_mult_cpu_thread, v1, v2, v1->length/2, v1->length/2);
    t1.join();
    t2.join(); // */

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cpu_time_used = %lf sec\n", cpu_time_used);

    printf("scalar_mult_result = %lf\n", scalar_mult_result);

    return 0;
}