// gcc inner_product_files_bin_generation.c -o inner_product_files_bin_generation
// ./inner_product_files_bin_generation

#include <stdio.h>
#include <string.h>// strlen
#include "vector1d.c"

int main(int argc, char *argv[])
{
    printf("*** Inner Product Files Binary Generation ***\n");

    // Инициализация параметров приложения
    unsigned long long N  = 10000000ULL;// Количество элементов
    unsigned long long Nb = 1000000ULL; // Количество элементов в блоке памяти
    int min = -10;
    int max = 15;
    double k = 1.0;

    const char* file_name_v1 = "v1.bin";
    const char* file_name_v2 = "v2.bin";

    // Обработка параметров командной строки
    for (int i = 0; i < argc; i++)
    {
        // Обработка пар параметров
        if(i+1 >= argc) break;
        
        if(strstr(argv[i],"-nb"))
        {                 
            size_t value = atoll(argv[i+1]);
            if(value > 0)
            {
                printf("command: %s | value = %ld\n", argv[i], value);
                Nb = value;
                i++;
            }
        }
        else if(strstr(argv[i],"-n"))
        {
            unsigned long long value = atoll(argv[i+1]);
            if(value <= 0) { printf("CONTINUE\n");}            
            printf("command: %s | value = %lld\n", argv[i], value);
            N = value;
            i++;            
        }
        else if(strstr(argv[i],"-min"))
        {
            double value = atof(argv[i+1]);
            if(fabs(value)<0.00000001) continue;
            printf("command: %s | value = %lf\n", argv[i], value);
            min = value;
            i++;            
        }
        else if(strstr(argv[i],"-max"))
        {
            double value = atof(argv[i+1]);
            if(fabs(value)<0.00000001) continue;
            printf("command: %s | value = %lf\n", argv[i], value);
            max = value;
            i++;            
        }
        else if(strstr(argv[i],"-k"))
        {
            double value = atof(argv[i+1]);
            if(fabs(value)<0.00000001) continue;
            printf("command: %s | value = %lf\n", argv[i], value);
            k = value;
            i++;            
        }
        else if(strstr(argv[i],"-fn1"))
        {            
            printf("command: %s | value = %s\n", argv[i], argv[i+1]);
            file_name_v1 = argv[i+1];
            i++;            
        }
        else if(strstr(argv[i],"-fn2"))
        {            
            printf("command: %s | value = %s\n", argv[i], argv[i+1]);
            file_name_v2 = argv[i+1];
            i++;            
        }
            
    }
    
    
    double inner_product = 0;
    if(!inner_product_files_bin_generate(file_name_v1, file_name_v2, N, Nb, min, max, k, &inner_product))
    {
        printf("ERROR!");
        exit(1);
    }

    printf("inner_product = %lf\n", inner_product);
    
    vector1d* v1 = NULL;
    if(!vector1d_create_from_file(file_name_v1, &v1))
    {
        printf("Error in reading file %s!\n", file_name_v1);
        exit(1);
    }
    //vector1d_print(v1, "v1 = ");

    vector1d* v2 = NULL;
    if(!vector1d_create_from_file(file_name_v2, &v2))
    {
        printf("Error in reading file %s!\n", file_name_v2);
        exit(1);
    }
    //vector1d_print(v2, "v2 = ");

    long double inner_product_calculated = scalar_mult(v1, v2);
    if(inner_product_calculated > 0) printf("inner_product_calculated>0\n");
    printf("inner_product_calculated = %Lf\n", (double)inner_product_calculated);
    
    return 0;
}
