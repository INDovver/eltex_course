#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "calc.h"

void sub(bool* go_on,double *res, int *count,...){
    clearCons();
    va_list arg;
    va_start(arg, count);
        if (*go_on==false){
            if(*count==1){
                *res-=va_arg(arg,double);
            }
            else{
                *res = va_arg(arg, double);
            }
            for (int i = 1; i < *count; i++){
                *res-=va_arg(arg, double);
            }
            *go_on=true;
        }
        else if (*count == 0){
            return;
        }
        else{
            for (int i = 0; i < *count; i++){
                *res-=va_arg(arg, double);
            }
        }
    va_end(arg);
}