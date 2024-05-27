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
    int gpu_sm_max;// Количество потоковых мультипроцессоров графического ускорителя, ед.
} comp_system_gpu;


/// @brief Структура "Узел гетерогенной вычислительной системы"
typedef struct
{
    double ram_size_gb;// Объём доступной оперативной памяти, Гб
    int cpu_threads_max;// Максимальное количество потоков, исполняемых на центральном процессоре (потоков CPU), ед.
    int gpu_num;// Количество графических ускорителей
    comp_system_gpu** gpus;// Массив указателей на структуру comp_system_gpu
} comp_system_node;
//////////////////////////////////////////////////////////////

/////////////////////// Создание /////////////////////////////

/// @brief Создаёт структуру "Технические характеристики графического ускорителя"
/// @param vram_size_gb Объём доступной видеопамяти, Гб
/// @param gpu_sm_max Количество потоковых мультипроцессоров графического ускорителя, ед.
/// @return 
comp_system_gpu* comp_system_gpu_create(double vram_size_gb, int gpu_sm_max)
{
    comp_system_gpu* gpu = (comp_system_gpu*)malloc(sizeof(comp_system_gpu));
    gpu->vram_size_gb = vram_size_gb;
    gpu->gpu_sm_max = gpu_sm_max;
    return gpu;
}

/// @brief Создаёт массив графических ускорителей
/// @param gpu_num Количество графических ускорителей в вычислительной системе
/// @return Указатель на массив графических ускорителей
comp_system_gpu** comp_system_gpu_array_create(int gpu_num)
{
    comp_system_gpu** comp_system_gpu_array = (comp_system_gpu**)malloc(gpu_num * sizeof(comp_system_gpu*));
    return comp_system_gpu_array;
}

/// @brief Создаёт структуру "Узел гетерогенной вычислительной системы"
/// @param ram_size_gb Объём доступной оперативной памяти, Гб
/// @param cpu_threads_max Максимальное количество потоков, исполняемых на центральном процессоре (потоков CPU), ед.
/// @param gpu_num Количество графических ускорителей
/// @param gpus Массив указателей на структуру comp_system_gpu
/// @return Указатель на созданную структуру
comp_system_node* comp_system_node_create(double ram_size_gb,
    int cpu_threads_max, int gpu_num, comp_system_gpu** gpus)
{
    comp_system_node* node = (comp_system_node*)malloc(sizeof(comp_system_node));
    node->ram_size_gb = ram_size_gb;
    node->cpu_threads_max = cpu_threads_max;
    node->gpu_num = gpu_num;
    node->gpus = (comp_system_gpu**)malloc(node->gpu_num * sizeof(comp_system_gpu*));
    node->gpus = gpus;
    return node;
}

/// @brief Создаёт структуру "Узел гетерогенной вычислительной системы" с одним графическим ускорителем
/// @param ram_size_gb Объём доступной оперативной памяти, Гб
/// @param cpu_threads_max Максимальное количество потоков, исполняемых на центральном процессоре (потоков CPU), ед.
/// @param vram_size_gb Объём доступной видеопамяти, Гб
/// @param gpu_sm_max Количество потоковых мультипроцессоров графического ускорителя, ед.
/// @return Указатель на созданную структуру
comp_system_node* comp_system_node_create_1GPU(double ram_size_gb, int cpu_threads_max,
    double vram_size_gb, int gpu_sm_max)
{
    comp_system_gpu* gpu = comp_system_gpu_create(vram_size_gb, gpu_sm_max);
    comp_system_gpu** comp_system_gpu_array = comp_system_gpu_array_create(1);
    comp_system_gpu_array[0] = gpu;
    comp_system_node* node = comp_system_node_create(ram_size_gb, cpu_threads_max, 1, comp_system_gpu_array);
}

//////////////////////////////////////////////////////////////

////////////// Вывод сведений в консоль  /////////////////////

/// @brief Выводит в консоль параметры видеокарты
/// @param message Сообщение
/// @param gpu Указатель на структуру "Технические характеристики gpu" (comp_system_gpu)
void comp_system_gpu_print(const char* message, comp_system_gpu* gpu)
{
    printf("%s", message);
    printf("%lf Gb; ", gpu->vram_size_gb);
    printf("%d SMs; ", gpu->gpu_sm_max);
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
/////////////////////////////////////////////////////////////////

#endif