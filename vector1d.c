#ifndef VECTOR1D_C

#define VECTOR1D_C

#include <stdio.h>
#include <stdlib.h> // malloc
#include <time.h> // rand, srand
#include <math.h>

typedef struct
{
    int length;
    double *data;
} vector1d;

vector1d* vector1d_create(int size)
{
    vector1d *vec1d = (vector1d*)malloc(sizeof(vector1d));
    vec1d->length = size;
    //printf("vec1d->length = %d\n", vec1d->length);
    vec1d->data = (double*)malloc(vec1d->length * sizeof(double));

    return vec1d;
}

void vector1d_init_scalar(vector1d *v1, vector1d *v2, double k, int min, int max)
{
    int rnd;
    srand(time(NULL));
        
    for(size_t i = 0; i<v1->length; i++)
    {
        double rnd = min + rand() % (max-min);        
        v1->data[i] = k*rnd;
        v2->data[i] = 1/rnd;      
    }
}

void vector1d_print(vector1d *v, const char* message)
{
    printf("%s", message);
    for(size_t i = 0; i<v->length; i++)
    {        
        printf("%lf ", v->data[i]);         
    }
    printf("\n");
}

double scalar_mult(vector1d *v1, vector1d *v2)
{
    double res = 0;

    for(size_t i = 0; i < v1->length; i++)
    {        
        res += v1->data[i] * v2->data[i];         
    }

    return res;
}

#endif