#ifndef VECTOR1D_C

#define VECTOR1D_C

#include <stdio.h>
#include <stdlib.h> // malloc
#include <time.h> // rand, srand
#include <math.h>

typedef struct
{
    unsigned long long length;
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

/// @brief Освобождает память, занятую структурой vector1d
/// @param v1 Адрес указателя на структуру vector1d
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

/// @brief Инициализирует векторы v1 и v2 таким образом, чтобы произведение v1[i]*v2[i]=k
/// @param v1 Указатель на первый вектор
/// @param v2 Указатель на второй вектор
/// @param ind_start Индекс, с которого начинается инициализация
/// @param length Длина области инциализации - количество элементов
/// @param min Минимальное значение элементов вектора v1
/// @param max Максимальное значение элементов вектора v1
/// @param k Число, которому должно быть равно произведение v1[i]*v2[i]
/// @param inner_product Аналитическое значение скалярного произведения
/// @return Код успешности: 1 - ОК; 0 - ERROR
int vector1d_init_vectors_for_inner_product_testing_range(vector1d* v1, vector1d* v2,
    size_t ind_start, size_t length,
    double min, double max, double k, long double* inner_product)
{    
    if(!v1) return 0;
    if(!v2) return 0;
    if(v1->length != v2->length) return 0;
    if(ind_start + length > v1->length) return 0;

    vector1d_init_random_double(v1, min, max);
    
    for(size_t i = ind_start; i < ind_start+length; i++)
    {
        if(fabs(v1->data[i]) < 0.00000001)
            v2->data[i] = 0;
        else
            v2->data[i] = ((long double)k/v1->data[i]);
    }

    *inner_product = k * v1->length;

    return 1;
}

/// @brief Инициализирует векторы v1 и v2 таким образом, чтобы произведение v1[i]*v2[i]=k
/// @param v1 Указатель на первый вектор
/// @param v2 Указатель на второй вектор
/// @param min Минимальное значение элементов вектора v1
/// @param max Максимальное значение элементов вектора v1
/// @param k Число, которому должно быть равно произведение v1[i]*v2[i]
/// @param inner_product Аналитическое значение скалярного произведения
/// @return Код успешности: 1 - ОК; 0 - ERROR
int vector1d_init_vectors_for_inner_product_testing(vector1d* v1, vector1d* v2,
    double min, double max, double k, long double* inner_product)
{
    if(!v1) return 0;
    if(!v2) return 0;
    if(v1->length != v2->length) return 0;

    if(!vector1d_init_vectors_for_inner_product_testing_range(v1, v2, 0, v1->length, min, max, k, inner_product))
        return 0;

    return 1;
}

/// @brief Вычисляет скалярное произведение векторов
/// @param v1 Указатель на первый вектор
/// @param v2 Указатель на второй вектор
/// @return Скалярное произведение
long double scalar_mult(vector1d *v1, vector1d *v2)
{
    long double res = 0;

    for(unsigned long long i = 0ULL; i < v1->length; i++)
    {        
        res += v1->data[i] * v2->data[i];         
    }

    return res;
}

/// @brief Вычисляет скалярное произведение непрерывного диапазона элементов векторов
/// @param v1 Указатель на первый вектор
/// @param v2 Указатель на второй вектор
/// @param ind_start Индекс первого элемента диапазона
/// @param length Длина диапазона
/// @return Скалярное произведение
long double scalar_mult_block(vector1d *v1, vector1d *v2,
    unsigned long long ind_start, unsigned long long length)
{
    long double res = 0;

    for(unsigned long long i = ind_start; i < length; i++)
    {        
        res += v1->data[i] * v2->data[i];         
    }

    return res;
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
	fwrite(&n, sizeof(v->length), 1, f); //Запись числа в двоичный файл.
	fwrite(v->data, sizeof(v->data), n, f); //Запись числа в двоичный файл.	
	fclose(f); //Закрыть файл.
}

/// @brief Создаёт вектор, считывая данные из двоичного файла
/// @param file_name имя файла
/// @param v адрес указателя на вектор типа vector1d**
/// @return Код успешности: 1 - ОК; 0 - ERROR
int vector1d_create_from_file(const char* file_name, vector1d** v)
{	
	FILE *f = fopen(file_name, "rb"); //Открыть существующий двоичный файл в режиме чтения.
    if(!f) return 0;

    unsigned long long n;
	fread(&n, sizeof((*v)->length), 1, f); //Читать из файла целое число в переменную n.
    if(n <= 0) return 0;

	*v = vector1d_create(n);
    (*v)->length = n;	
	//Чтение n вещественных чисел размером sizeof(double) каждое из файла f в массив arr.
	fread((*v)->data, sizeof((*v)->data), n, f);	
	fclose(f); //Закрыть файл.

	return 1;
}

/// @brief Создаёт вектор указанного размера, считывая данные из двоичного файла с указанной позиции
/// @param f Указатель на двоичный файл, открытый в режиме чтения
/// @param index_start Индекс первого считываемого элемента
/// @param length Количество элементов
/// @param v Адрес указателя на вектор типа vector1d**
/// @return Код успешности: 1 - ОК; 0 - ERROR
int vector1d_create_from_file_pos(FILE *f, unsigned long long index_start,
    unsigned long long length, vector1d** v)
{
    unsigned long long pos = sizeof((*v)->length) + index_start * sizeof((*v)->data);
    printf("Reading from pos %lld\n", pos);
	if(fseek(f, pos, SEEK_SET))
    {
        printf("Error in fseek!\n");
        return 0;
    }

	*v = vector1d_create(length);
    (*v)->length = length;	
	//Чтение n чисел размером sizeof((*v)->data) каждое из файла f в массив (*v)->data.
	fread((*v)->data, sizeof((*v)->data), length, f);

	return 1;
}

int vector1d_init_from_file_pos(FILE *f, unsigned long long index_start,
    unsigned long long length, vector1d* v)
{
    unsigned long long pos = sizeof(v->length) + index_start * sizeof(v->data);
    //printf("Reading from pos %lld\n", pos);

    unsigned long long seek_block_size = 1000000000Ull;
    if(pos > seek_block_size)
    {
        if(fseek(f, seek_block_size, SEEK_SET))
        {
            printf("Reading from pos %lld\n", pos);
            printf("Error in fseek!\n");
            return 0;
        }
        for(size_t i = 1; i < pos / seek_block_size; i++)
        {
            if(fseek(f, seek_block_size, SEEK_CUR))
            {
                printf("Reading from pos %lld\n", pos);
                printf("Error in fseek!\n");
                return 0;
            }
        }

        if(fseek(f, pos % seek_block_size, SEEK_CUR))
        {
            printf("Reading from pos %lld\n", pos);
            printf("Error in fseek!\n");
            return 0;
        }

        //printf("Reading from pos %lld\n", pos);
        //printf("pos > 10^9!\n");        
    }
    else
    {
        if(fseek(f, pos, SEEK_SET))
        {
            printf("Reading from pos %lld\n", pos);
            printf("Error in fseek!\n");
            return 0;
        }
    }
	
	//Чтение n чисел размером sizeof(v->data) каждое из файла f в массив v->data.
	fread(v->data, sizeof(v->data), length, f);

	return 1;
}

/// @brief Создаёт два файла двоичного формата, первым значением является количество элементов, далее - элементы
/// @param file_name_v1 Имя первого файла
/// @param file_name_v2 Имя второго файла
/// @param N Количество элементов
/// @param Nb Количество элементов в блоке памяти
/// @param min Минимальное значение элементов вектора v1
/// @param max Максимальное значение элементов вектора v1
/// @param k Число, которому должно быть равно произведение v1[i]*v2[i]
/// @param inner_product Аналитическое значение скалярного произведения
/// @return Код успешности: 1 - ОК; 0 - ERROR
int inner_product_files_bin_generate(const char* file_name_v1, const char* file_name_v2,
    unsigned long long N, unsigned long long Nb, int min, int max, double k, double* inner_product)
{
    // Создаём блоки памяти
    vector1d* v1 = vector1d_create(Nb);
    vector1d* v2 = vector1d_create(Nb);
    *inner_product = 0;
        
    //printf("Saving 2 vector1d into files %s and %s...\n", file_name_v1, file_name_v1);
    
    FILE *f_v1, *f_v2; //Описание файловых переменных.	
	f_v1 = fopen(file_name_v1, "wb"); //Создание двоичного файла в режиме записи.
    if(!f_v1)
    {
        printf("Error opening file %s\n", file_name_v1);
        return 0;
    }

    f_v2 = fopen(file_name_v2, "wb"); //Создание двоичного файла в режиме записи.
    if(!f_v2)
    {
        printf("Error opening file %s\n", file_name_v2);
        return 0;
    }
    
	unsigned long long n = N;
	fwrite(&n, sizeof(unsigned long long), 1, f_v1); //Запись числа в двоичный файл.
    fwrite(&n, sizeof(unsigned long long), 1, f_v2); //Запись числа в двоичный файл.
	
    for (unsigned long long i = 0; i < N/Nb; i++)
    {
        long double inner_product_block = 0;
        // Заполняем блоки v1 и v2       
        if(!vector1d_init_vectors_for_inner_product_testing(v1, v2, min, max, k, &inner_product_block))
        {
            printf("ERROR in vector1d_init_vectors_for_inner_product_testing!");
            return 0;
        }
        *inner_product += inner_product_block;
        //printf("inner_product_block = %lf\n", inner_product_block);
        //printf("inner_product must be %lf\n", *inner_product);

        /*vector1d_print(v1, "block v1 = ");
        vector1d_print_details(v1, "block v1 details: ");
        vector1d_print(v2, "block v2 = ");
        vector1d_print_details(v2, "block v2 details: "); //*/

        fwrite(v1->data, sizeof(double), v1->length, f_v1); //Запись числа в двоичный файл.
        fwrite(v2->data, sizeof(double), v2->length, f_v2); //Запись числа в двоичный файл.
    }
    vector1d_free(&v1);
    vector1d_free(&v2);

    unsigned long long ost = N % Nb;
    if(ost)
    {
        vector1d* v1_ost = vector1d_create(ost);
        vector1d* v2_ost = vector1d_create(ost);
        long double inner_product_block = 0;
        // Заполняем блоки v1 и v2       
        if(!vector1d_init_vectors_for_inner_product_testing(v1_ost, v2_ost, min, max, k, &inner_product_block))
        {
            printf("ERROR in vector1d_init_vectors_for_inner_product_testing!");
            return 0;
        }
        *inner_product += inner_product_block;
        //printf("inner_product_block = %lf\n", inner_product_block);
        //printf("inner_product must be %lf\n", *inner_product);

        /*vector1d_print(v1_ost, "block v1_ost = ");
        vector1d_print_details(v1_ost, "block v1_ost details: ");
        vector1d_print(v2_ost, "block v2_ost = ");
        vector1d_print_details(v2_ost, "block v2_ost details: "); //*/

        fwrite(v1_ost->data, sizeof(double), v1_ost->length, f_v1); //Запись числа в двоичный файл.
        fwrite(v2_ost->data, sizeof(double), v2_ost->length, f_v2); //Запись числа в двоичный файл.

        vector1d_free(&v1_ost);
        vector1d_free(&v2_ost);
    }
        
    fclose(f_v1); //Закрыть файл.
    fclose(f_v2); //Закрыть файл.

    return 1;
}

/////////// Файловые операции (КОНЕЦ) //////////////


#endif