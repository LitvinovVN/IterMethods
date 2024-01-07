// g++ algorithms_inner_product_tests.cpp -o algorithms_inner_product_tests -fopenmp
// ./algorithms_inner_product_tests

#include <iostream>
#include "vector1d.c"
#include "algorithms_inner_product.cpp"

int main()
{
    std::cout << "*** Algorithms Inner Product Tests ***" << std::endl;

    int inner_product_algorithms_number = 3;
    // Массив указателей на функции, вычисляющие скалярное произведение векторов
    inner_product_result (*inner_product_algorithms[inner_product_algorithms_number])(inner_product_params) = {
        inner_product_cpu_1,
        inner_product_cpu_n_threads,
        inner_product_cpu_OpenMP      
    };

    size_t N = 100000000;
    vector1d* v1 = vector1d_create(N);
    vector1d_init_random_double(v1, -50, 50);
    vector1d* v2 = nullptr;
    double inner_product;
    if(!vector1d_init_v2_for_inner_product_testing(v1, &v2, 0.5, &inner_product))
    {
        std::cout << "ERROR in vector1d_init_v2_for_inner_product_testing!" << std::endl;
        return 1;
    }
    std::cout << "inner_product must be " << inner_product << std::endl;
    
    inner_product_params params;
    params.v1 = v1->data;
    params.v2 = v2->data;
    params.n = N;
    params.nt_cpu = 4;
    params.nt_gpu = 0;

    for(int i = 0; i < inner_product_algorithms_number; i++)
    {
        std::cout << "TEST " << i+1 << " from " << inner_product_algorithms_number << ": ";
        inner_product_result ipres = inner_product_algorithms[i](params);
        inner_product_result_print(ipres, "");        

        std::cout << "abs. error: " << ipres.result - inner_product << " | ";
        std::cout << "otn. error: " << (ipres.result - inner_product)/inner_product << std::endl;        
    }
      

    return 0;
}