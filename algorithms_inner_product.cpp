#pragma once

#include <chrono>
#include "vector1d.c"
#include <thread>
#include <mutex>
#include <vector>

std::mutex mutex;

/// @brief Структура параметров алгоритмов вычисления скалярного произведения векторов
typedef struct
{
    double* v1;// указатель на первый массив
    double* v2;// указатель на второй массив
    size_t n; // размерность векторов 
    size_t nt_cpu;// количество потоков CPU, задействуемых в процессе вычислений
    size_t nt_gpu; // количество потоковых мультипроцессоров, задействуемых в процессе вычислений
} inner_product_params;

/// @brief Структура результатов вычисления скалярного произведения векторов
typedef struct
{
    bool is_success;// флаг успешности вычислений
    double result;// результат вычисления
    double time; // время вычислений
} inner_product_result;

void inner_product_result_print(inner_product_result ipr, const char* message)
{
    printf("%s", message);
    if(!ipr.is_success)
    {
        printf("ERROR!\n");
        return;
    }

    printf("OK | res=%lf | %lf sec.\n", ipr.result, ipr.time);
}






/// @brief Вычисляет скалярное произведение векторов последовательно
/// @param params Структура параметров
/// @param results Структура результатов
/// @return скалярное произведение
inner_product_result inner_product_cpu_1(inner_product_params params)
{
    auto start = std::chrono::system_clock::now();

    double res = 0;

    for(size_t i = 0; i < params.n; i++)
    {        
        res += params.v1[i] * params.v2[i];
        //std::cout << i << ": res = " << res << " | params.v1[i] = " << params.v1[i] << " | params.v2[i] = " << params.v2[i] << std::endl;        
    }

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    
    inner_product_result return_res;
    return_res.is_success = true;
    return_res.result = res;
    return_res.time = dur.count();

    return return_res;
}
///////////////////////////////////////////////////////////////////////////



// Функция вычисления скалярного произведения
// *v1 - указатель на вектор 1
// *v2 - указатель на вектор 2
// startInd - индекс элемента, с которого будет рассчитывать свой фрагмент текущий поток
// length - количество элементов (размер фрагмента), которое будет рассчитывать текущий поток
// result - ссылка на переменную, куда будет записан результат вычисления скалярного произведения
void scalar_mult_cpu_thread(double *v1, double *v2, size_t startInd, size_t length, double& result)
{
    //printf("scalar_mult_cpu_thread\n");
    double temp = 0;
    for(size_t i = startInd; i < startInd+length; i++)
    {
        temp += v1[i] * v2[i];
    }

    mutex.lock();
    result += temp;
    mutex.unlock();
}

/// @brief Вычисление скалярного произведения на CPU в многопоточном режиме
/// @param params 
/// @return 
inner_product_result inner_product_cpu_n_threads(inner_product_params params)
{
    auto start = std::chrono::system_clock::now();
    
    size_t n = params.n;
    auto v1 = params.v1;
    auto v2 = params.v2;
    double res = 0;
    int NumCpuThreads = params.nt_cpu;
    std::vector<std::thread> threads;

    if(n % NumCpuThreads == 0)//Если количество узлов делится на количество потоков нацело, размеры данных для каждого потока будут одинаковы
    {
        int NbyThread = n / NumCpuThreads;
        //printf("NbyThread = %d\n", NbyThread);
        for(size_t i = 0; i < NumCpuThreads; i++)
            threads.push_back(std::thread(scalar_mult_cpu_thread, v1, v2, i*NbyThread, NbyThread, std::ref(res)));
    }
    else
    {
        int NbyThread = n / (NumCpuThreads-1);
        int NbyThreadLast = n % (NumCpuThreads-1);
        //printf("NbyThread = %d, NbyThreadLast = %d\n", NbyThread, NbyThreadLast);

        for(size_t i = 0; i < NumCpuThreads-1; i++)
            threads.push_back(std::thread(scalar_mult_cpu_thread, v1, v2, i*NbyThread, NbyThread, std::ref(res)));

        threads.push_back(std::thread(scalar_mult_cpu_thread, v1, v2, (NumCpuThreads-1)*NbyThread, NbyThreadLast, std::ref(res)));
    }

    for(auto& thread : threads)
    {
        if(thread.joinable())
            thread.join();
    }
    
    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    
    inner_product_result return_res;
    return_res.is_success = true;
    return_res.result = res;
    return_res.time = dur.count();

    return return_res;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include <omp.h>

/// @brief Вычисляет скалярное произведение векторов с помощью OpenMP
/// @param params Структура параметров
/// @param results Структура результатов
/// @return скалярное произведение
inner_product_result inner_product_cpu_OpenMP(inner_product_params params)
{
    auto start = std::chrono::system_clock::now();
    double res = 0.;
    omp_set_num_threads(params.nt_cpu);
#pragma omp parallel for reduction(+:res)
    for (int i = 0; i < params.n; ++i)
    {
//#pragma omp atomic
        res += (params.v1[i] * params.v2[i]);
    }

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    
    inner_product_result return_res;
    return_res.is_success = true;
    return_res.result = res;
    return_res.time = dur.count();

    return return_res;
}
///////////////////////////////////////////////////////////////////////////


////////////////////////////// CUDA ///////////////////////////////////////
/*extern int inner_product_cuda(double* v1, double* v2, const int n, double* result, double* time);

/// @brief Вычисляет скалярное произведение векторов последовательно на GPU
/// @param params Структура параметров
/// @param results Структура результатов
/// @return скалярное произведение
inner_product_result inner_product_gpu_1(inner_product_params params)
{
    auto start = std::chrono::system_clock::now();

    double res = 0;
    double time_gpu = 0;
    inner_product_cuda(params.v1, params.v2, params.n, &res, &time_gpu);

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    
    inner_product_result return_res;
    return_res.is_success = true;
    return_res.result = res;
    return_res.time = dur.count();

    return return_res;
}*/
///////////////////////////////////////////////////////////////////////////