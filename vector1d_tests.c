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

    // Прибавляемо всем элементам вектора v1 случайное число в диапазоне от 0.000 до 0.999
    vector1d_add_random_0_1_3digit(v1);
    vector1d_print(v1, "v1 (15.000...15.999) = ");

    return 0;
}