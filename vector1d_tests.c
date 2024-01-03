// gcc vector1d_tests.c -o vector1d_tests
// ./vector1d_tests
#include <stdio.h>
#include "vector1d.c"

int main()
{
    printf("*** vector1d tests ***\n");
    vector1d* v1 = vector1d_create(10);
    vector1d_print(v1, "v1 = ");
    vector1d_print_details(v1, "v1 details: ");

    // Инициализируем вектор v1 целыми числами в диапазоне от -10 до 20
    vector1d_init_random(v1, -10, 20);
    vector1d_print(v1, "v1 (-10...20) = ");

    // Инициализируем вектор v1 нулями
    vector1d_init_zeroes(v1);
    vector1d_print(v1, "v1 (0) = ");

    // Инициализируем вектор v1 целыми числами в диапазоне от -5 до 5
    vector1d_init_random(v1, 5, -5);
    vector1d_print(v1, "v1 (-5...5) = ");

    // Инициализируем вектор v1 числом 15
    vector1d_init_random(v1, 15, 15);
    vector1d_print(v1, "v1 (15) = ");

    // Прибавляем ко всем элементам вектора v1 случайное число в диапазоне от 0.000 до 0.999
    vector1d_add_random_0_1_3digit(v1);
    vector1d_print(v1, "v1 (15.000...15.999) = ");

    // Инициализируем вектор v1 целыми числами в диапазоне от 10.000 до 14.999.
    vector1d_init_random_double(v1, 10, 15);
    vector1d_print(v1, "v1 (10.0...14.999) = ");

    vector1d* v2 = NULL;
    if(vector1d_clone(v1, &v2))
    {
        printf("v1 copied into v2\n");
    }
    else
    {
        printf("ERROR! v1 not copied into v2!\n");
    }

    // Сравниваем v1 и v2
    if(vector1d_is_equals(v1, v2))
        printf("OK: v1 = v2\n");
    else
        printf("ERROR!: v1 != v2\n");
    // Удаляем вектор v1
    vector1d_free(&v1);
    printf("v1 addr after free: %p\n", v1);
    // Удаляем вектор v2
    vector1d_free(&v2);
    printf("v2 addr after free: %p\n", v2);


    double inner_product = 0;
    v1 = vector1d_create(10000);
    vector1d_init_random_double(v1, -1000, 1000);
    if(!vector1d_init_v2_for_inner_product_testing(v1, &v2, 0.01, &inner_product))
        printf("ERROR in vector1d_init_v2_for_inner_product_testing function!");
    else
        printf("inner_product = %lf\n",inner_product);

    double inn_prod = scalar_mult(v1, v2);
    if(fabs(inn_prod - inner_product) < 0.00000001)
        printf("OK: %lf = %lf\n", inn_prod, inner_product);
    else
        printf("ERROR!: %lf != %lf\n", inn_prod, inner_product);

    vector1d_print_range(v1, "v1", 20, 10);
    vector1d_print_range(v2, "v2", 20, 10);
    printf("0.01: %lf * %lf = %lf\n", v1->data[20], v2->data[20], v1->data[20] * v2->data[20]);

    printf("*** vector1D tests completed ***\n");
    return 0;
}