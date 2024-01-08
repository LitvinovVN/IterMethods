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

    unsigned long long N = n1; // Количество элементов
    unsigned long long Nb = 100000000ULL; // Количество элементов в блоке памяти
    if(N < Nb)
        Nb = N;
    std::cout   << "N = "  << N  << "; "
                << "Nb = " << Nb << "; "
                << std::endl;

    // Создаём векторы-буферы для хранения блоков данных
    vector1d* v1_buff = vector1d_create(Nb);
    vector1d* v2_buff = vector1d_create(Nb);

    long double inner_product = 0;
    for (unsigned long long  block_ind = 0; block_ind < N/Nb; block_ind++)
    {
        unsigned long long  pos = block_ind*N;
        if(!vector1d_init_from_file_pos(f1, pos, Nb, v1_buff))
        {
            std::cout << "Error in vector1d_init_from_file_pos (v1_buff)" << std::endl;
            exit(1);
        }        

        if(!vector1d_init_from_file_pos(f2, pos, Nb, v2_buff))
        {
            std::cout << "Error in vector1d_init_from_file_pos (v2_buff)" << std::endl;
            exit(1);
        }

        //vector1d_print(v1_buff,"v1_buff = ");
        //vector1d_print(v2_buff,"v2_buff = ");

        long double inner_product_block = scalar_mult(v1_buff, v2_buff);
        //std::cout << block_ind << ": ";
        //std::cout << "inner_product_block = " << inner_product_block << std::endl;       
        inner_product += inner_product_block;
    }    
    if(N % Nb)
    {
        size_t pos = (N/Nb)*N;
        if(!vector1d_init_from_file_pos(f1, pos, N % Nb, v1_buff))
        {
            std::cout << "Error in vector1d_create_from_file_pos" << std::endl;
            exit(1);
        }        

        if(!vector1d_init_from_file_pos(f2, pos, N % Nb, v2_buff))
        {
            std::cout << "Error in vector1d_create_from_file_pos" << std::endl;
            exit(1);
        }

        //vector1d_print(v1_buff,"v1_buff = ");
        //vector1d_print(v2_buff,"v2_buff = ");

        long double inner_product_block = scalar_mult_block(v1_buff, v2_buff, 0, N % Nb);
        //std::cout << "inner_product_block = " << inner_product_block << std::endl;       
        inner_product += inner_product_block;
    }
    std::cout << "inner_product = " << inner_product << std::endl;       

    // Закрываем файлы
    fclose(f1);
    fclose(f2);

    // Освобождаем память
    vector1d_free(&v1_buff);
    vector1d_free(&v2_buff);

    return 0;
}