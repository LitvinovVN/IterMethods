#ifndef VECTOR1D_C

#define VECTOR1D_C

#include <stdio.h>
#include <stdlib.h> // malloc
#include <time.h> // rand, srand
#include <math.h>

typedef struct
{
    int length;
    double *data;
} vector1d;

//////////////////////// Создание  //////////////////////////////////
vector1d* vector1d_create(int size)
{
    vector1d *vec1d = (vector1d*)malloc(sizeof(vector1d));
    vec1d->length = size;
    //printf("vec1d->length = %d\n", vec1d->length);
    vec1d->data = (double*)malloc(vec1d->length * sizeof(double));

    return vec1d;
}
//////////////////////// Создание (КОНЕЦ)  ///////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////// Инициализация  //////////////////////////////

/// @brief Инициализирует все элементы вектора v значением value
/// @param v указатель на инициализируемый вектор типа vector1d*
/// @param value целочисленное значение, которое будет присвоено всем элементам вектора v
void vector1d_init_int(vector1d* v, int value)
{
    for(size_t i = 0; i < v->length; i++)
    {
        v->data[i] = value;
    }
}

/// @brief Инициализирует все элементы вектора v значением 0
/// @param v указатель на инициализируемый вектор типа vector1d*
void vector1d_init_zeroes(vector1d* v)
{
    vector1d_init_int(v, 0);
}

/// @brief Инициализирует все элементы вектора v случайными целыми числами в диапазоне от min до max
/// @param v указатель на инициализируемый вектор типа vector1d*
/// @param min минимальное значение
/// @param max максимальное значение
void vector1d_init_random(vector1d* v, int min, int max)
{
    if(min == max)
    {
        vector1d_init_int(v, min);
        return;
    }

    if(min > max)
    {
        vector1d_init_random(v, max, min);
        return;
    }

    srand(time(0));
    for(size_t i = 0; i < v->length; i++)
    {
        int num = min + (rand() % (max - min + 1));
        v->data[i] = num;
    }    
}

/// @brief Добавляет к текущему значению каждого элемента вектора v случайное число в диапазоне от 0.000 до 0.999
/// @param v указатель на модифицируемый вектор типа vector1d*
void vector1d_add_random_0_1_3digit(vector1d* v)
{
    srand(time(0));
    int d = 1000;
    for(size_t i = 0; i < v->length; i++)
    {
        double num = (rand() % d) / (double)d;
        v->data[i] = v->data[i]+num;
    }  
}


void vector1d_init_scalar(vector1d *v1, vector1d *v2, double k, int min, int max)
{
    int rnd;
    srand(time(NULL));
        
    for(size_t i = 0; i<v1->length; i++)
    {
        double rnd = min + rand() % (max-min);        
        v1->data[i] = k*rnd;
        v2->data[i] = 1/rnd;      
    }
}

////////////// Инициализация (КОНЕЦ)  ///////////////////
/////////////////////////////////////////////////////////



void vector1d_print(vector1d *v, const char* message)
{
    printf("%s", message);
    for(size_t i = 0; i<v->length; i++)
    {        
        printf("%lf ", v->data[i]);         
    }
    printf("\n");
}

/// @brief Выводит в консоль детализированную информацио о векторе
/// @param v указатель на вектор типа vector1d*
/// @param message сообщение, выводимое перед детализированным выводом
void vector1d_print_details(vector1d *v, const char* message)
{
    printf("%s", message);
    printf("v->length = %d | ", v->length);
    printf("v->data = %p\n", v->data);
}

/// @brief Вычисляет скалярное произведение векторов
/// @param v1 указатель на первый вектор
/// @param v2 указатель на второй вектор
/// @return скалярное произведение
double scalar_mult(vector1d *v1, vector1d *v2)
{
    double res = 0;

    for(size_t i = 0; i < v1->length; i++)
    {        
        res += v1->data[i] * v2->data[i];         
    }

    return res;
}

#endif