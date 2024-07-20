#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>


#define MF 20


typedef struct op{
    void (*operation) (bool*, double* , int*,...);
    char name[MF];
}op;


void clearCons() {
  for (int i = 0; i < 50; ++i) {
    printf("\n");
  }
}


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


void mult(bool* go_on,double *res, int *count,...){
    clearCons();
    va_list arg;
    va_start(arg, count);
        if (*go_on==false){
            if(*count==1){
                *res*=va_arg(arg,double);
            }
            else if (*count == 0){
                return;
            }
            else{
                *res = va_arg(arg, double);
            }
            for (int i = 1; i < *count; i++){
                *res*=va_arg(arg, double);
            }
            *go_on=true;
        }
        else{
            for (int i = 0; i < *count; i++){
                *res*=va_arg(arg, double);
            }
        }
    va_end(arg);
}


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


void getstr(int size, int *choice, double* array, int* cnt, op* oper){
    char c;
    printf("Введите номерр операции и числа, над которыми необходимо выполнить операцию через пробел\nЕсли это первое действие и введено одно число, то действие выполнится над нулём\nQ) Выйти\nR) Сбросить\n");
    for (int i =0; i<size;i++){
        printf("%d) %s\n", i+1, oper[i].name); 
    }

    c = getchar();
    if (c=='r'||c=='q'||c=='R'||c=='Q') {
        while (getchar() != '\n'); 
        *choice = tolower(c);
        return;
    }
    
    else if (c == '\n'){
        clearCons();
        printf("Ничего не введено!\n");
        return;
    }

    else if(c>'0'&& c<=size+'0'){
        ungetc(c, stdin);
        scanf("%d", choice);
        clearCons();
    }

    else {
        while (getchar() != '\n');
        clearCons();
        printf("Выбор несуществующей команды!\n");
        return;
    }

    while (*cnt < MF){
        c = getchar();
        if (c == '\n'){
            break;
        }
        else {
            ungetc(c, stdin);
            *cnt+=scanf("%lf", &array[*cnt]);
        }
    }
}


void main(void) {

    op oper[] = {{add, "Сложение"},{sub, "Вычетания"},{mult, "Умножение"},{div, "Деление"}};
    int choice,  count;
    double args[MF], res = 0;
    bool go_on = false;
    clearCons();
    while(true){
        choice = count = 0;
        if (go_on == true){
            if (res == (int)res){
                printf("Текущее значение: %d\n", (int)res);
            }
            else{
                printf("Текущее значение: %.2f\n", res);
            }
        }
        getstr(sizeof(oper)/sizeof(oper[0]), &choice, args, &count, oper);
        switch(count){
            case 1:oper[choice-1].operation(&go_on, &res, &count, args[0]); break;
            case 2:oper[choice-1].operation(&go_on, &res, &count,args[0],args[1]); break;
            case 3:oper[choice-1].operation(&go_on, &res, &count,args[0],args[1],args[2]); break;
            case 4:oper[choice-1].operation(&go_on, &res, &count,args[0],args[1],args[2],args[3]); break;
            case 5:oper[choice-1].operation(&go_on, &res, &count,args[0],args[1],args[2],args[3],args[4]); break;
            default: switch (choice){
                case 'q': return;
                case 'r': go_on=false; res = 0; clearCons(); break;
                default: printf("Нет аргументов?\n"); break;
            };
        }
    }
}