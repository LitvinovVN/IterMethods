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

/// @brief Инициализирует все элементы вектора v случайными вещественными числами в диапазоне от min до max
/// @param v указатель на инициализируемый вектор типа vector1d*
/// @param min минимальное значение
/// @param max максимальное значение
void vector1d_init_random_double(vector1d* v, int min, int max)
{
    if(min == max)
    {
        vector1d_init_int(v, min);
        return;
    }

    if(min > max)
    {
        vector1d_init_random_double(v, max, min);
        return;
    }

    srand(time(0));
    int d = 1000;
    for(size_t i = 0; i < v->length; i++)
    {
        int num = min + (rand() % (max - min));
        double frac = (rand() % d) / (double)d;
        v->data[i] = num + frac;
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

void vector1d_print_range(vector1d *v, const char* message, size_t indStart, size_t length)
{
    printf("%s [%ld...%ld]: ", message, indStart, indStart+length-1);
    for(size_t i = indStart; i < indStart+length; i++)
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


/////////////// Файловые операции //////////////////
/// @brief Записывает вектор v в файл file_name
/// @param file_name имя файла
/// @param v указатель на вектор типа vector1d*
void vector1d_save_to_file_bin(const char* file_name, vector1d* v)
{
    printf("Saving vector1d into file %s...\n", file_name);
    
    FILE *f; //Описание файловой переменной.	
	f = fopen(file_name, "wb"); //Создание двоичного файла в режиме записи.
	size_t n = v->length;
	fwrite(&n, sizeof(int), 1, f); //Запись числа в двоичный файл.
	fwrite(v->data, sizeof(double), n, f); //Запись числа в двоичный файл.	
	fclose(f); //Закрыть файл.
}

/// @brief Создаёт вектор, считывая данные из двоичного файла
/// @param file_name имя файла
/// @param v адрес указателя на вектор типа vector1d**
/// @return 
int vector1d_create_from_file(const char* file_name, vector1d** v)
{	
	FILE *f = fopen(file_name, "rb"); //Открыть существующий двоичный файл в режиме чтения.
    if(!f) return 1;

    int n;
	fread(&n, sizeof(int), 1, f); //Читать из файла целое число в переменную n.
    if(n <=0) return 2;

	*v = vector1d_create(n);
    (*v)->length = n;	
	//Чтение n вещественных чисел размером sizeof(double) каждое из файла f в массив arr.
	fread((*v)->data, sizeof(double), n, f);	
	fclose(f); //Закрыть файл.

	return 0;
}

/////////// Файловые операции (КОНЕЦ) //////////////

/// @brief Сравнивает два вектора
/// @param v1 указатель на первый вектор
/// @param v2 указатель на второй вектор
/// @return 1 - равны, 0 - не равны
int vector1d_is_equals(vector1d* v1, vector1d* v2)
{
    if(v1->length != v2->length)
        return 0;

    for(size_t i = 0; i < v1->length; i++)
    {
        if(fabs(v1->data[i] - v2->data[i]) > 0.00000001)
            return 0;
    }
    
    return 1;
}

void vector1d_free(vector1d** v1)
{
    free((*v1)->data);
    (*v1)->data = NULL;
    free(*v1);
    *v1 = NULL;
}

/// @brief Клонирует вектор v1 в вектор v2
/// @param v1 указатель на вектор-источник, адрес не равен 0
/// @param v2 указатель на вектор-приёмник, адрес равен 0
/// @return 1 - успех, 0 - ошибка
int vector1d_clone(vector1d* v1, vector1d** v2)
{    
    if(!v1) return 0;
    if(*v2) return 0;

    *v2 = vector1d_create(v1->length);

    for(size_t i = 0; i < v1->length; i++)
        (*v2)->data[i] = v1->data[i];

    return 1;
}

/// @brief Инициализирует вектор v2 таким образом, чтобы произведение v1[i]*v2[i]=k
/// @param v1 Указатель на первый вектор
/// @param v2 Адрес указателя на второй вектор
/// @param k Число, которому должно быть равно произведение v1[i]*v2[i]
/// @param inner_product Аналитическое значение скалярного произведения
/// @return Код успешности: 1 - ОК; 0 - ERROR
int vector1d_init_v2_for_inner_product_testing(vector1d* v1, vector1d** v2, double k, double* inner_product)
{    
    if(!v1) return 0;
    if(*v2) return 0;

    *v2 = vector1d_create(v1->length);

    for(size_t i = 0; i < v1->length; i++)
    {
        if(fabs(v1->data[i]) < 0.00000001)
            (*v2)->data[i] = 0;
        else
            (*v2)->data[i] = k/v1->data[i];
    }

    *inner_product = k * v1->length;

    return 1;
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