#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "calc.h"

void div(bool* go_on,double *res, int *count,...){
    clearCons();
    va_list arg;
    va_start(arg, count);
        if (*go_on==false){
            if (*count==1){
                *res /= va_arg(arg, double);
            }
            else if (*count == 0){
                return;
            }
            else{
                *res = va_arg(arg, double);
            }
            for (int i = 1; i < *count; i++){
                double tmp = va_arg(arg, double);
                if (tmp == 0){
                    printf("Деление на ноль\n");
                    return;
                }
                else{
                    *res/=tmp;
                }
            }
            *go_on=true;
        }
        else{
            for (int i = 0; i < *count; i++){
                double tmp = va_arg(arg, double);
                if (tmp == 0){
                    printf("Деление на ноль\n");
                    return;
                }
                else{
                    *res/=tmp;
                }
            }
        }
    va_end(arg);
}