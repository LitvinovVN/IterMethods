// gcc vecMult.c -o app -std=c99
// ./app

#include "vector1d.c"

int main()
{
    size_t size = 2000000;
    
    vector1d *v1 = vector1d_create(size);
    printf("*v1 = %p\n", v1);
    printf("v1->length = %d\n", v1->length);

    vector1d *v2 = vector1d_create(size);
    printf("*v2 = %p\n", v2);    
    printf("v2->length = %d\n", v2->length);

    double k = 0.5;
    int min = 10;
    int max = 20;

    clock_t start, end;
    double cpu_time_used;
     
    vector1d_init_scalar(v1, v2, k, min, max);
    //vector1d_print(v1);
    //vector1d_print(v2);

    start = clock();
    double sm = scalar_mult(v1, v2);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cpu_time_used = %lf sec\n", cpu_time_used);

    printf("scalar_mult = %lf\n", sm);

    return 0;
}