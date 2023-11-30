// g++ vecMult.cpp -o app
// ./app

#include "vector1d.c"
#include <thread>
#include <mutex>

std::mutex mutex;
double scalar_mult_result = 0;

void scalar_mult_cpu_thread(vector1d *v1, vector1d *v2, size_t startInd, size_t length)
{
    printf("scalar_mult_cpu_thread\n");
    double temp = 0;
    for(size_t i = startInd; i < startInd+length; i++)
    {
        temp += v1->data[i] * v2->data[i];
    }

    mutex.lock();
    scalar_mult_result += temp;
    mutex.unlock();
}

int main()
{
    size_t size = 40000000;
    
    vector1d *v1 = vector1d_create(size);
    printf("*v1 = %p\n", v1);
    printf("v1->length = %d\n", v1->length);

    vector1d *v2 = vector1d_create(size);
    printf("*v2 = %p\n", v2);    
    printf("v2->length = %d\n", v2->length);

    double k = 0.5;
    int min = 10;
    int max = 20;

    clock_t start, end;
    double cpu_time_used;
        
    vector1d_init_scalar(v1, v2, k, min, max);
    //vector1d_print(v1);
    //vector1d_print(v2);
    
    start = clock();

    std::thread t1(scalar_mult_cpu_thread, v1, v2, 0, v1->length); // + Вычисление скалярного произведения одном потоком
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