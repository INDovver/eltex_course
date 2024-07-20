#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MF 20     // Max Field length
#define MC 10     // Max Contats
#define MPS 5     // Max Phone and Social media numbers  


typedef struct Job {
    char Organization[MF];
    char Post[MF];
} Job;


typedef struct SocialMedia {
    char Email[MPS][MF];             // Адресса электронных почт
    char SocialNetwork[MPS][MF];     // Ссылки на страницы в соцсетях
    char Messenger[MPS][MF];         // ССылки на профили в мессенджерах
} SocialMedia;


typedef struct Person {
    char LastName[MF];    
    char FirstName[MF];   
    char Patronymic[MF];        // Отчество
    char PhoneNumber[MPS][MF]; 
    SocialMedia PersonSM;
    Job PersonJob;                        
} Person;


typedef struct btree{
    Person p;
    struct btree *left, *right;
}btree;


void clearCons() {
  for (int i = 0; i < 50; ++i) {
    printf("\n");
  }
}


int get_curn(char (*str)[MF]){
    int curn=-1;
    for (int j=0; j<MPS;j++){
        if (strlen(str[j])!=0){
            curn = j;
        }
        else{
            return curn;
        }
    }
}


void addField(char str[MF], char text[]){
    printf("%s", text);
    fgets(str, MF, stdin);
    str[strcspn(str, "\n")] = 0;
}


void addCase(char (*str)[MF]){
    int curn = get_curn(str);
    clearCons();
    if (strlen(str[MPS-1]) == 0){ 
        printf("Введите новое значение: ");
        fgets(str[curn+1], MF, stdin);
        str[curn+1][strcspn(str[curn+1], "\n")] = 0;
        clearCons();
        return;
    }
    else {
        printf("Нет места\n");
        return;
    }
    return;
}


void editCase(char (*str)[MF]){
    int curn = get_curn(str);
    char numc[MF];
    int num;
    printf("Какой элемент изменить?: "); 
    fgets(numc, MF, stdin);
    num = atoi(numc)-1;
    if (num<0 || num>curn){
        clearCons();
        printf("Такого элемента нет\n");
        return;
    }
    else {
        clearCons();
        printf("Введите новое значение: ");
        fgets(str[num], MF, stdin);
        str[num][strcspn(str[num], "\n")] = 0;
        clearCons();
        return;
    }
}


void deleteCase(char (*str)[MF]){
    int curn = get_curn(str);
    char numc[MF];
    int num;
    printf("Какой элемент удалить?: ");
    fgets(numc, MF, stdin);
    num = atoi(numc)-1;
    if (num<0 || num>curn){
        clearCons();
        printf("Такого элемента нет\n");
        return;
    }
    else {
        clearCons();
        for (num; num <MPS;num++){
            if (strlen(str[num+1])!=0){
                strncpy(str[num], str[num+1],MF);
            }
            else{
                memset(str[num],0,MF);
            }
        }
        return;
    }
}


char entEdit(){
    char text = getchar();
    while (getchar() != '\n'); 
    text = tolower(text);
    return text;
}


void editContact(btree *el, char *last){
    if(el==NULL){
        printf("Такого контакта нет!\n");
        return;
    }
    if(strcmp(el->p.LastName,last)==0){
    while (true){
        printf("Что хотите изменить?\nN - ФИО\nJ - Работу\nP - Номера телефонов\nS - Соц. сети\nQ - Ничего\n");
        switch(entEdit()){
            default:
                clearCons();
                break;
            case 'n':
                clearCons();
                do {
                    addField(el->p.LastName, "Введите Фамилию: ");
                    clearCons();
                    if (strlen(el->p.LastName) == 0) {
                    clearCons();
                    printf("Ошибка! Фамилия не может быть пустой.\n");
                    }       
                } while (strlen(el->p.LastName) == 0);
                do {
                    addField(el->p.FirstName, "Введите Имя: ");
                    clearCons();
                    if (strlen(el->p.FirstName) == 0) {
                    clearCons();
                    printf("Ошибка! Имя не может быть пустым.\n");
                    }       
                } while (strlen(el->p.FirstName) == 0);
                do {
                    addField(el->p.Patronymic, "Введите Отчество: ");
                    clearCons();
                    if (strlen(el->p.Patronymic) == 0) {
                    clearCons();
                    printf("Ошибка! Отчество не может быть пустым.\n");
                    }       
                } while (strlen(el->p.Patronymic) == 0);
                break;
            case 'j':
                clearCons();
                addField(el->p.PersonJob.Organization, "Введите место работы: ");
                clearCons();

                addField(el->p.PersonJob.Post, "Введите должность: ");
                clearCons();
                break;
            case 'p':
                clearCons();
                printf("Текущие номера телефонов:\n");
                for (int j=0; j<MPS;j++){
                    if (strlen(el->p.PhoneNumber[j])!=0){
                        printf("%d) %s\n", j+1, el->p.PhoneNumber[j]);
                    }
                    else{
                        break;
                    }
                }
                printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                switch (entEdit()){
                    case 'a':
                        addCase(el->p.PhoneNumber);
                        break;
                    case'c':
                        editCase(el->p.PhoneNumber);
                        break;
                    case 'd':
                        deleteCase(el->p.PhoneNumber);
                        break;
                    case 'n':
                        clearCons();
                        break;
                    default: clearCons();
                }
                break;
            case 's':
                clearCons();
                printf("Что хотите изменить?\nE - имейл\nS - соц. сети\nM - мессенджер\n");
                switch (entEdit()){
                    case 'e':
                        clearCons();
                        printf("Текущие почтовые ящики:\n");
                        for (int j=0; j<MPS;j++){
                            if (strlen(el->p.PersonSM.Email[j])!=0){
                                printf("%d) %s\n", j+1, el->p.PersonSM.Email[j]);
                            }
                            else{
                                break;
                            }
                        }
                        printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                        switch (entEdit()){
                            case 'a':
                                addCase(el->p.PersonSM.Email);
                                break;
                            case'c':
                                editCase(el->p.PersonSM.Email);
                                break;
                            case 'd':
                                deleteCase(el->p.PersonSM.Email);
                                break;
                            case 'n':
                                clearCons();
                                break;
                            default: clearCons();
                        }
                        break;
                    case 'm':
                        clearCons();
                        printf("Текущие почтовые ящики:\n");
                        for (int j=0; j<MPS;j++){
                            if (strlen(el->p.PersonSM.Messenger[j])!=0){
                                printf("%d) %s\n", j+1, el->p.PersonSM.Messenger[j]);
                            }
                            else{
                                break;
                            }
                        }
                        printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                        switch (entEdit()){
                            case 'a':
                                addCase(el->p.PersonSM.Messenger);
                                break;
                            case'c':
                                editCase(el->p.PersonSM.Messenger);
                                break;
                            case 'd':
                                deleteCase(el->p.PersonSM.Messenger);
                                break;
                            case 'n':
                                clearCons();
                                break;
                            default: clearCons();
                        }
                        break;
                    case 's':
                        clearCons();
                        printf("Текущие соц. сети:\n");
                        for (int j=0; j<MPS;j++){
                            if (strlen(el->p.PersonSM.SocialNetwork[j])!=0){
                                printf("%d) %s\n", j+1, el->p.PersonSM.SocialNetwork[j]);
                            }
                            else{
                                break;
                            }
                        }
                        printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                        switch (entEdit()){
                            case 'a':
                                addCase(el->p.PersonSM.SocialNetwork);
                                break;
                            case'c':
                                editCase(el->p.PersonSM.SocialNetwork);
                                break;
                            case 'd':
                                deleteCase(el->p.PersonSM.SocialNetwork);
                                break;
                            case 'n':
                                clearCons();
                                break;
                            default: clearCons();
                        }
                        break;
                }
                break;
            case 'q':
                return;
        
        }
    }
    }
    if(strcmp((el)->p.LastName,last)>0) return editContact((el)->left, last);
    return editContact((el)->right, last);
}


int showFullInfo(btree *ptr, char *last){
    if(ptr==NULL){
        printf("Такого контакта нет!\n");
        return 1;
    }
    if(strcmp(ptr->p.LastName,last)==0){
    printf("ФИО: %s %s %s\nМесто работы: %s, Должность: %s\n", ptr->p.LastName, ptr->p.FirstName, 
    ptr->p.Patronymic, ptr->p.PersonJob.Organization, ptr->p.PersonJob.Post);
    printf("Номера телефонов:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(ptr->p.PhoneNumber[j])!=0){
            printf("%s\n",ptr->p.PhoneNumber[j]);
        }
    }
    printf("Электронные почты:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(ptr->p.PersonSM.Email[j])!=0){
            printf("%s\n",ptr->p.PersonSM.Email[j]);
        }
    }  
    printf("Аккаунты соц. сетей:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(ptr->p.PersonSM.SocialNetwork[j])!=0){
            printf("%s\n",ptr->p.PersonSM.SocialNetwork[j]);
        }
    }
    printf("Аккаунты в мессенджерах:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(ptr->p.PersonSM.Messenger[j])!=0){
            printf("%s\n",ptr->p.PersonSM.Messenger[j]);
        }
    }
    return 0;
    }
    if(strcmp((ptr)->p.LastName,last)>0) return showFullInfo((ptr)->left, last);
    return showFullInfo((ptr)->right, last);
}


Person init_p(){
        static int id = 1;
        Person p;
        memset(&p,0,sizeof(Person));
        clearCons();
        while (strlen(p.LastName) == 0){
            addField(p.LastName, "Введите Фамилию: ");
            if (strlen(p.LastName) == 0) {
            clearCons();
            printf("Ошибка! Фамилия не может быть пустой.\n");
            }       
        }
        clearCons();
        while (strlen(p.FirstName) == 0){
            addField(p.FirstName, "Введите имя: ");
            if (strlen(p.FirstName) == 0) {
            clearCons();
            printf("Ошибка! Имя не может быть пустым.\n");
            }       
        }
        clearCons();
        while (strlen(p.Patronymic) == 0){
            addField(p.Patronymic, "Введите отчество: ");
            if (strlen(p.Patronymic) == 0) {
            clearCons();
            printf("Ошибка! Отчество не может быть пустым.\n");
            }       
        }
        clearCons();
        printf("Хотите добавить место работы и должность? Y/N: ");

        switch (entEdit()){
            case 'y':
                clearCons();
                addField(p.PersonJob.Organization, "Введите место работы: ");
                clearCons();
                addField(p.PersonJob.Post, "Введите должность: ");
                break;

            case 'n':
                break;
        }
        clearCons();
        printf("Хотите добавить номер телефона? Y/N: ");
        char choice = entEdit();

        switch (choice){
            case 'y':
                clearCons();
                addField(p.PhoneNumber[0], "Введите номер телефона: ");
                int i=1;
                while((choice == 'y') && i<5){
                    clearCons();
                    printf("Хотите добавить ещё один номер телефона (max %d)? Y/N: ", MPS);
                    choice = entEdit();
                    switch (choice){
                        case 'y':
                            clearCons();
                            addField(p.PhoneNumber[i], "Введите номер телефона: ");
                            i++;
                            break;
                        case 'n':
                            break;
                    }
                }
                break;
            case 'n':
                break;
        }
        clearCons();
        printf("Хотите добавить социальные сети? Y/N: ");
        choice = entEdit();

        switch (choice){
            case 'y':
                while(choice == 'y'){
                    int ind = -1;
                    printf("Что хотите добавить?\nE - имейл\nS - социальные сети\nM - мессенджер\nD - Завершить\n");
                    switch (entEdit()){
                        case 'e':
                            for (int i = 0; i<MPS; i++){
                                if (strlen(p.PersonSM.Email[i])==0){
                                    ind = i;
                                    break;
                                }
                            }
                            if (ind != -1){
                                clearCons();
                                addField(p.PersonSM.Email[ind], "Введите электронную почту: ");
                            }
                            else{
                                clearCons();
                                printf("Места для почт не осталось\n");
                                break;
                            }
                            break;

                        case 's':
                            for (int i = 0; i<MPS; i++){
                                if (strlen(p.PersonSM.SocialNetwork[i])==0){
                                    ind = i;
                                    break;
                                }
                            }
                            if (ind != -1){
                                clearCons();
                                addField(p.PersonSM.SocialNetwork[ind], "Введите социальную сеть и свой логин: ");
                            }
                            else{
                                clearCons();
                                printf("Места социальных сетей не осталось\n");
                                break;
                            }
                            break;

                        case 'm':
                            for (int i = 0; i<MPS; i++){
                                if (strlen(p.PersonSM.Messenger[i])==0){
                                    ind = i;
                                    break;
                                }
                            }
                            if (ind != -1){
                                clearCons();
                                addField(p.PersonSM.Messenger[ind], "Введите мессенджер и свой логин: ");
                            }
                            else{
                                clearCons();
                                printf("Места социальных сетей не осталось\n");
                                break;
                            }
                            break;

                        case  'd':
                            clearCons();
                            choice = 'n';
                            break;

                        default:
                            clearCons();
                    }
                }
            case 'n':
                clearCons();
                break;
        }
        return p;
}


void deleteContact(btree **head, char *last){
    btree *tmp, *up;
    if (*head==NULL){
        printf("Такого контакта нет!");
        return;
    } 
    if(strcmp((*head)->p.LastName,last)==0){
        if (((*head)->left==NULL) && ((*head)->right==NULL)){
            free(*head);
            (*head)=NULL;
            return;
        }
        if((*head)->left==NULL){
            tmp = *head;
            (*head)=(*head)->right;
            free(tmp);
            return;
        }
        if((*head)->right==NULL){
            tmp = *head;
            (*head)=(*head)->left;
            free(tmp);
            return;
        }
        up = *head;
        tmp=(*head)->left;
        while (tmp->right!=NULL){
            up=tmp;
            tmp=tmp->right;
        }
        (*head)->p = tmp->p;
        if (up!=(*head)){
            if(tmp->left!=NULL) up->right=tmp->left;
            else up->right = NULL;
        }
        else (*head)->left=tmp->left;
        free(tmp);
        return;
    }
    if(strcmp((*head)->p.LastName,last)>0) return deleteContact(&(*head)->left, last);
    return deleteContact(&(*head)->right, last);
}


void addContact(btree **head, Person pers){
    if(*head==NULL){
        *head=malloc(sizeof(btree));
        (*head)->left=(*head)->right=NULL;
        (*head)->p=pers;
        return;
    }
    if(strcmp((*head)->p.LastName,pers.LastName)>0) addContact(&(*head)->left,pers);
    else addContact(&(*head)->right,pers);
}


void print_btree(btree *b){
    if (b==NULL) return;
    print_btree(b->left);
    printf("%s %s %s\n", b->p.LastName, b->p.FirstName, b->p.Patronymic);
    print_btree(b->right);
}


int main (){
    clearCons();
    btree *cntct = NULL;
    
    while (true){
        printf("A - добавить\nQ - выйти\nдля взаимедействия с контактом выберие его порядковый номер\n");
        print_btree(cntct);
        char inp[MF]={0};
        fgets(inp, MF, stdin);
        inp[strcspn(inp, "\n")]=0;
        clearCons();
            if(inp[1]==0){
            inp[0]=tolower(inp[0]);
            switch (inp[0]){
                case 'a':
                    addContact(&cntct, init_p());
                    break;
                case 'q':
                    return 0;
                default: printf("Такой операции нет\n"); 
            }
            continue;
            }
            int succ = showFullInfo(cntct, inp);
            if (succ) continue;
            printf("Меню действий над контактом:\nD - удалить\nC - изменить\nN - не изменять\n");
            switch(entEdit()){
                case 'd':
                    deleteContact(&cntct, inp);
                    clearCons();
                    break;
                case 'n':
                    clearCons();
                    break;
                case 'c':
                    clearCons();
                    editContact(cntct, inp);
                    clearCons();
                    break;
            }
    }
}