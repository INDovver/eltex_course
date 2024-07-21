#include "calc.h"
#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>

int main(void) {
    void *handleadd = dlopen("./libcalcadd.so",RTLD_LAZY);
    if (!handleadd) {
        fputs (dlerror(), stderr);
        return 1;
    }
    void *handlesub = dlopen("./libcalcsub.so",RTLD_LAZY);
    if (!handlesub) {
        fputs (dlerror(), stderr);
        return 1;
    }
    void *handlemult = dlopen("./libcalcdiv.so",RTLD_LAZY);
    if (!handlemult) {
        fputs (dlerror(), stderr);
        return 1;
    }
    void *handlediv = dlopen("./libcalcmult.so",RTLD_LAZY);
    if (!handlediv) {
        fputs (dlerror(), stderr);
        return 1;
    }

    op oper[] = {{dlsym(handleadd, "add"), "Сложение"},{dlsym(handlesub, "sub"), "Вычетания"},{dlsym(handlemult, "mult"), "Умножение"},{dlsym(handlediv, "div"), "Деление"}};
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
                case 'q': return 0;
                case 'r': go_on=false; res = 0; clearCons(); break;
                default: printf("Нет аргументов?\n"); break;
            };
        }
    }
    dlclose(handlesub);
    dlclose(handleadd);
    dlclose(handlediv);
    dlclose(handlemult);
    return 0;
}