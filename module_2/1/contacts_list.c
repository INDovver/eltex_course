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


typedef struct list{
    Person p;
    int ID;   // В этой реализации ID - скорее просто порядковый номер при выводе списка
    struct list *next;
    struct list *prev;
}list;


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


void editContact(list *head, list *el){
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
                        }
                        break;
                }
                break;
            case 'q':
                return;
        
        }
    }
}


list *showFullInfo(list *head, int id){
    list *ptr = head;
    while(ptr!=NULL&&ptr->ID!=id){
        ptr=ptr->next;
    }
    if(ptr==NULL){
        printf("Такого контакта нет!\n");
        return NULL;
    }
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
    return ptr;
}


Person init_p(){
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


void deleteContact(list **head, list *el){
    if(el->prev!=NULL){
        el->prev->next=el->next;
    }
    else{
        *head=el->next;
    }
    if(el->next!=NULL){
        el->next->prev=el->prev;
    }
    list *tmp = el;
    while(tmp!=NULL){
        tmp->ID--;
        tmp=tmp->next;
    }
    free(el);
}


void addContact(struct list **head, Person pers){ 
    list *new = malloc(sizeof(list));
    new->p=pers;
    list *tmp = *head;
    new->next=NULL;
    new->prev=NULL;
    new->ID = 1;
    if (*head == NULL){
        *head = new;
        return;
    }
    while(strcmp(pers.LastName, tmp->p.LastName) >= 0) { 
        if (tmp->next!=NULL){
            tmp = tmp->next;
        }
        else{
            new->prev=tmp;
            new->ID=tmp->ID+1;
            tmp->next=new; 
            return;
        }
    }
    if (tmp!=(*head)){
        new->prev=tmp->prev;
        new->next=tmp;
        tmp->prev->next=new;
        tmp->prev=new;
        new->ID=tmp->ID;
    }
    else{
        new->prev=NULL;
        new->next=tmp;
        new->ID=tmp->ID; 
        tmp->prev=new;
        *head=new;
    }
    while(tmp!=NULL){
        tmp->ID++;
        tmp=tmp->next;
    }
}


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