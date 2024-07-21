#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "calc.h"

void clearCons() {
  for (int i = 0; i < 50; ++i) {
    printf("\n");
  }
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
