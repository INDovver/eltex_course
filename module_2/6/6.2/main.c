#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "contacts.h"
#include "dlfcn.h"

int main (){
    clearCons();
    list *cntct = NULL;
    
    while (true){
        printf("A - добавить\nQ - выйти\nдля взаимедействия с контактом выберие его порядковый номер\n");
        for (list *i=cntct; i!=NULL; i=i->next){
            printf("head указывает на %s\n", cntct->p.LastName);
            printf("%d) %s %s %s\n", i->ID, i->p.LastName, i->p.FirstName, i->p.Patronymic);
        }
        char inp[MF];
        fgets(inp, MF, stdin);
        clearCons();
        int id = atoi(inp); 
        if (id == 0){
            inp[0]=tolower(inp[0]);
            switch (inp[0]){
                case 'a':
                    addContact(&cntct, init_p());
                    break;
                case 'q':
                    return 0;
            }
        }
        else if (id > 0) {
            list *curel = showFullInfo(cntct, id);
            if(curel==NULL) continue; 
            printf("Меню действий над контактом:\nD - удалить\nC - изменить\nN - не изменять\n");
            switch(entEdit()){
                case 'd':
                    deleteContact(&cntct, curel);
                    clearCons();
                    break;
                case 'n':
                    clearCons();
                    break;
                case 'c':
                    clearCons();
                    editContact(cntct, curel);
                    clearCons();
                    break;
            }
        } 
    }
}