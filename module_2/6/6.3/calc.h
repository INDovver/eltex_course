#include <stdbool.h>
#ifndef CALC_H
#define CALC_H

#define MF 20

typedef struct op{
    void (*operation) (bool*, double* , int*,...);
    char name[MF];
}op;

void clearCons();


void add(bool* go_on,double *res, int *count,...);


void sub(bool* go_on,double *res, int *count,...);


void mult(bool* go_on,double *res, int *count,...);


void div(bool* go_on,double *res, int *count,...);


void getstr(int size, int *choice, double* array, int* cnt, op* oper);

#endif