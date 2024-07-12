// Метод правой монотонной прогонки
// g++ progonka_prav.cpp -o progonka_prav
// ./progonka_prav 1000
#include <iostream>
#include <string>

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

int main(int argc, char* argv[])
{    
    if (argc < 2)
    {
        std::cout << "You need enter command line argument n!" << std::endl;
        exit(-1);
    }
    std::cout << "Method pravoy monotonnoy progonki" << std::endl;
    size_t n = std::stoull(argv[1]);
    std::cout << "n = " << n << std::endl;    
    
    // Выделение памяти под массивы
    double* a = new double[n];
    double* b = new double[n];
    double* c = new double[n];
    double* y = new double[n];
    double* y_izv = new double[n];
    double* f = new double[n];

    // Инициализируем массивы     
    a[0] = 0;
    b[0] = 2;
    c[0] = -1;
    y_izv[0] = 1.0;
    for(int i = 1; i < n-1; i++)
    {
        a[i] = -1.0;
        b[i] =  2.0;
        c[i] = -1.0;
        y_izv[i] = i + 1.0;        
    }
    a[n-1] = -1.0;
    b[n-1] = 2.0;
    c[n-1] = 0.0;
    y_izv[n-1] = n;

    f[0] = b[0]*y_izv[0] + c[0]*y_izv[1];
    for(int i = 1; i < n-1; i++)
    {
        f[i] = a[i]*y_izv[i-1] + b[i]*y_izv[i] + c[i]*y_izv[i+1];
    }
    f[n-1] = a[n-1]*y_izv[n-2] + b[n-1]*y_izv[n-1];

    //print_arrays(n, a, b, c, y_izv, f);
}