#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "calc.h"

void add(bool* go_on,double *res, int *count,...){
    clearCons();
    va_list arg;
    va_start(arg, count);
    if (*count == 0){
        return;
    }
    for (int i = 0; i < *count; i++){
        *res+=va_arg(arg, double);
    }
    va_end(arg);
    *go_on=true;
}
