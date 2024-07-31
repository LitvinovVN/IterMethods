// Метод правой монотонной прогонки.
// Решение m систем из n уравнений
// g++ progonka_prav_threads.cpp -o progonka_prav_threads
// ./progonka_prav_threads 10 1000
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

void print_arrays(size_t n, double* a, double* b, double* c, double* y, double* f)
{
    for(int i = 0; i < n; i++)
    {
        std::cout   << i << ": "
                    << a[i] << "\t"
                    << b[i] << "\t"
                    << c[i] << "\t"
                    << y[i] << "\t"
                    << f[i] << "\n";
    }
}

/// @brief Правая монотонная прогонка
/// @param n Размерность СЛАУ 
/// @param a Массив коэффициентов диагонали -1
/// @param b Массив коэффициентов диагонали 0
/// @param c Массив коэффициентов диагонали 1
/// @param f Массив правых частей СЛАУ
/// @param y Вектор решений
void progonka_r(size_t n, const double* a, const double* b,
                 const double* c, const double* f, double* y)
{
    // Выделяем память для хранения прогоночных коэффициентов
    double* alfa = new double[n];
    double* beta = new double[n];

    //std::cout << "STEP1" << std::endl;
    // 1. Прямой ход прогонки
    //      а) Инициализация
    double delta = b[0];
    beta[0] = f[0]/delta;
    //      б)
    for(size_t i = 1; i < n; i++)
    {
        alfa[i-1] = -c[i-1]/delta;
        delta = b[i] + a[i] * alfa[i-1];
        beta[i] = (f[i] - a[i] * beta[i-1])/delta;
    }
    //std::cout << "STEP2" << std::endl;
    // 2. Обратный ход прогонки
    //      а) Инициализация
    y[n-1] = beta[n-1];
    //std::cout << "y[n-1] = " << y[n-1] << std::endl;    
    //      б)
    for(long long i = n-2; i > -1; i--)
    {
        y[i] = alfa[i] * y[i+1] + beta[i];
        //std::cout << "y[" << i <<"] = " << y[i] << std::endl;
    }

    delete[] alfa;
    delete[] beta;
}

/// @brief Проверка содержимого массивов на эквивалентность
/// @param y_izv 
/// @param y 
/// @return Возвращает максимальную абсолютную ошибку
double arrays_double_abs_error_max(size_t n, const double* y_izv, const double* y)
{    
    double max_error = 0;
    for(size_t i = 0; i < n; i++ )
    {
        double error = fabs(y_izv[i] - y[i]);
                    
        if(error > max_error)
            max_error = error;        
    }  

    return max_error;
}

int main(int argc, char* argv[])
{    
    if (argc < 3)
    {
        std::cout << "You need enter command line arguments m and n!" << std::endl;
        exit(-1);
    }
    std::cout << "Method pravoy monotonnoy progonki. Threads" << std::endl;
    size_t m = std::stoull(argv[1]);
    size_t n = std::stoull(argv[2]);
    std::cout << "m = " << m << std::endl;
    std::cout << "n = " << n << std::endl;  
    
    // Выделение памяти под массивы
    double* a = new double[m*n];
    double* b = new double[m*n];
    double* c = new double[m*n];
    double* y = new double[m*n];
    double* y_izv = new double[m*n];
    double* f = new double[m*n];

    // Инициализируем массивы     
    a[0] = 0;
    b[0] = -2;
    c[0] = 1;
    y_izv[0] = 1.0;
    for(int i = 1; i < n-1; i++)
    {
        a[i] = 1.0;
        b[i] =  -2.0;
        c[i] = 1.0;
        y_izv[i] = i%100 + 1;
    }
    a[n-1] = 1.0;
    b[n-1] = -2.0;
    c[n-1] = 0.0;
    y_izv[n-1] = n;

    f[0] = b[0]*y_izv[0] + c[0]*y_izv[1];
    for(int i = 1; i < n-1; i++)
    {
        f[i] = a[i]*y_izv[i-1] + b[i]*y_izv[i] + c[i]*y_izv[i+1];
    }
    f[n-1] = a[n-1]*y_izv[n-2] + b[n-1]*y_izv[n-1];

    if(n < 100)
        print_arrays(n, a, b, c, y_izv, f);

    std::cout << "Start... ";
    // Using time point and system_clock
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
 
    start = std::chrono::system_clock::now();
    progonka_r(n, a, b, c, f, y);
    end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    
    std::cout << "FINISH!" << std::endl;

    double max_error = arrays_double_abs_error_max(n, y_izv, y);
    std::cout << "max_error = " << max_error << std::endl;
    if(max_error < 0.000001)
        std::cout << "OK! Arrays are equals!";
    
}