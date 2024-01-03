#ifndef COMP_SYSTEM_C

#define COMP_SYSTEM_C

#include <stdio.h>
#include <stdlib.h> // malloc
#include <time.h> // rand, srand
#include <math.h>

/// @brief Структура "Технические характеристики графического ускорителя"
typedef struct
{
    double vram_size_gb;// Объём доступной видеопамяти, Гб
    int cpu_sm_max;// Количество потоковых мультипроцессоров графического ускорителя, ед.
} comp_system_gpu;


/// @brief Структура "Узел гетерогенной вычислительной системы"
typedef struct
{
    double ram_size_gb;// Объём доступной оперативной памяти, Гб
    int cpu_threads_max;// Максимальное количество потоков, исполняемых на центральном процессоре (потоков CPU), ед.
    int gpu_num;// Количество графических ускорителей
    comp_system_gpu** gpus;// Массив указателей на структуру comp_system_gpu
} comp_system_node;

////////////// Вывод сведений в консоль  /////////////////////
void comp_system_gpu_print(const char* message, comp_system_gpu* gpu)
{
    printf("%s", message);
    printf("%lf Gb; ", gpu->vram_size_gb);
    printf("%d SMs; ", gpu->cpu_sm_max);
    printf("\n");
}

void comp_system_node_print(const char* message, comp_system_node* node)
{
    printf("%s", message);
    printf("RAM: %lf Gb; ", node->ram_size_gb);
    printf("CPU threads max: %d; ", node->cpu_threads_max);
    for(int i=0; i < node->gpu_num; i++)
        comp_system_gpu_print("\tGPU: ", node->gpus[i]);
    printf("\n");
}

#endif