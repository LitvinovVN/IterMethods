// gcc vector1d_files_tests.c -o vector1d_files_tests
// ./vector1d_files_tests
#include <stdio.h>
#include "vector1d.c"



int main()
{
    printf("*** vector1d files tests ***\n");
    vector1d* v1 = vector1d_create(10);
    // Инициализируем вектор v1 целыми числами в диапазоне от 10.000 до 14.999.
    vector1d_init_random_double(v1, 10, 15);
    vector1d_print(v1, "v1 (10.0...14.999) = ");
    vector1d_print_details(v1, "v1 details: ");

    vector1d_save_to_file_bin("v1.bin", v1);

    vector1d* v2 = NULL;
    int fres = vector1d_create_from_file("v1.bin", &v2);
    printf("fres = %d\n", fres);
    vector1d_print(v2, "v2 = ");
    vector1d_print_details(v2, "v2 details: ");

    int isEquals = vector1d_is_equals(v1, v2);
    printf("isEquals = %d\n", isEquals);

    v2->data[0] = v2->data[0] + 0.00000001;    
    printf("isEquals = %d\n", vector1d_is_equals(v1, v2));

    return 0;
}