// Вычисление скалярного произведения из данных двоичных файлов
// g++ inner_product_files_bin.cpp -o inner_product_files_bin
// ./inner_product_files_bin

#include <iostream>
#include "vector1d.c"

int main()
{
    std::cout << "*** Inner Product Files Bin Tests ***" << std::endl;

    const char* file_name_v1 = "data/v1_10_10.bin";
    const char* file_name_v2 = "data/v2_10_10.bin";

    // Открываем существующие двоичные файлы в режиме чтения
    FILE *f1 = fopen(file_name_v1, "rb");
    if(!f1)
    {
        std::cout << "Error in opening file " << file_name_v1 << std::endl;
        exit(1);
    } 

    FILE *f2 = fopen(file_name_v2, "rb");
    if(!f2)
    {
        std::cout << "Error in opening file " << file_name_v1 << std::endl;
        exit(1);
    }

    // Считываем размеры векторов
    unsigned long long n1;
	fread(&n1, sizeof(unsigned long long), 1, f1); //Читать из файла целое число в переменную n.
    if(n1 <= 0)
    {
        std::cout << "Error in n1 <= 0: n1 = " << n1 << std::endl;
        exit(1);
    }

    unsigned long long n2;
	fread(&n2, sizeof(unsigned long long), 1, f2); //Читать из файла целое число в переменную n.
    if(n2 <= 0)
    {
        std::cout << "Error in n2 <= 0: n2 = " << n2 << std::endl;
        exit(1);
    }

    std::cout << "n1 = " << n1 << " | n2 = " << n2 << std::endl;
	
    // Если размеры разные, то ошибка
    if(n1 != n2)
    {
        std::cout << "Error in n1 != n2" << std::endl;
        exit(1);
    }

    vector1d* v1 = nullptr;
    if(!vector1d_create_from_file_pos(f1, 0, 10, &v1))
    {
        std::cout << "Error in vector1d_create_from_file_pos" << std::endl;
        exit(1);
    }

    vector1d* v2 = nullptr;
    if(!vector1d_create_from_file_pos(f2, 0, 10, &v2))
    {
        std::cout << "Error in vector1d_create_from_file_pos" << std::endl;
        exit(1);
    }
    
    vector1d_print(v1,"v1 = ");
    vector1d_print(v2,"v2 = ");
    double sm = scalar_mult(v1, v2);
    printf("sm = %lf\n", sm);

    // Закрываем файлы
    fclose(f1);
    fclose(f2); 

    return 0;
}