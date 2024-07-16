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
    int ID;
    char LastName[MF];    
    char FirstName[MF];   
    char Patronymic[MF];        // Отчество
    char PhoneNumber[MPS][MF]; 
    SocialMedia PersonSM;
    Job PersonJob;                        
} Person;


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


void editContact(Person array[], int ID){
    int i = ID - 1;
    int num = MPS+1;
    while (true){
        printf("Что хотите изменить?\nN - ФИО\nJ - Работу\nP - Номера телефонов\nS - Соц. сети\nQ - Ничего\n");
        switch(entEdit()){
            default:
                clearCons();
                break;
            case 'n':
                clearCons();
                do {
                    addField(array[i].LastName, "Введите Фамилию: ");
                    clearCons();
                    if (strlen(array[i].LastName) == 0) {
                    clearCons();
                    printf("Ошибка! Фамилия не может быть пустой.\n");
                    }       
                } while (strlen(array[i].LastName) == 0);
                do {
                    addField(array[i].FirstName, "Введите Имя: ");
                    clearCons();
                    if (strlen(array[i].FirstName) == 0) {
                    clearCons();
                    printf("Ошибка! Имя не может быть пустым.\n");
                    }       
                } while (strlen(array[i].FirstName) == 0);
                do {
                    addField(array[i].Patronymic, "Введите Отчество: ");
                    clearCons();
                    if (strlen(array[i].Patronymic) == 0) {
                    clearCons();
                    printf("Ошибка! Отчество не может быть пустым.\n");
                    }       
                } while (strlen(array[i].Patronymic) == 0);
                break;
            case 'j':
                clearCons();
                addField(array[i].PersonJob.Organization, "Введите место работы: ");
                clearCons();

                addField(array[i].PersonJob.Post, "Введите должность: ");
                clearCons();
                break;
            case 'p':
                clearCons();
                printf("Текущие номера телефонов:\n");
                for (int j=0; j<MPS;j++){
                    if (strlen(array[i].PhoneNumber[j])!=0){
                        printf("%d) %s\n", j+1, array[i].PhoneNumber[j]);
                    }
                    else{
                        break;
                    }
                }
                printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                switch (entEdit()){
                    case 'a':
                        addCase(array[i].PhoneNumber);
                        break;
                    case'c':
                        editCase(array[i].PhoneNumber);
                        break;
                    case 'd':
                        deleteCase(array[i].PhoneNumber);
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
                            if (strlen(array[i].PersonSM.Email[j])!=0){
                                printf("%d) %s\n", j+1, array[i].PersonSM.Email[j]);
                            }
                            else{
                                break;
                            }
                        }
                        printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                        switch (entEdit()){
                            case 'a':
                                addCase(array[i].PersonSM.Email);
                                break;
                            case'c':
                                editCase(array[i].PersonSM.Email);
                                break;
                            case 'd':
                                deleteCase(array[i].PersonSM.Email);
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
                            if (strlen(array[i].PersonSM.Messenger[j])!=0){
                                printf("%d) %s\n", j+1, array[i].PersonSM.Messenger[j]);
                            }
                            else{
                                break;
                            }
                        }
                        printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                        switch (entEdit()){
                            case 'a':
                                addCase(array[i].PersonSM.Messenger);
                                break;
                            case'c':
                                editCase(array[i].PersonSM.Messenger);
                                break;
                            case 'd':
                                deleteCase(array[i].PersonSM.Messenger);
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
                            if (strlen(array[i].PersonSM.SocialNetwork[j])!=0){
                                printf("%d) %s\n", j+1, array[i].PersonSM.SocialNetwork[j]);
                            }
                            else{
                                break;
                            }
                        }
                        printf("A - добавить\nC - изменить\nD - удалить\nN - Ничего не менять\n");
                        switch (entEdit()){
                            case 'a':
                                addCase(array[i].PersonSM.SocialNetwork);
                                break;
                            case'c':
                                editCase(array[i].PersonSM.SocialNetwork);
                                break;
                            case 'd':
                                deleteCase(array[i].PersonSM.SocialNetwork);
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


void showFullInfo(Person array[], int ID){
    int i = ID-1;
    printf("ФИО: %s %s %s\nМесто работы: %s, Должность: %s\n", array[i].LastName, array[i].FirstName, 
    array[i].Patronymic, array[i].PersonJob.Organization, array[i].PersonJob.Post);
    printf("Номера телефонов:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(array[i].PhoneNumber[j])!=0){
            printf("%s\n",array[i].PhoneNumber[j]);
        }
    }
    printf("Электронные почты:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(array[i].PersonSM.Email[j])!=0){
            printf("%s\n",array[i].PersonSM.Email[j]);
        }
    }
    printf("Аккаунты соц. сетей:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(array[i].PersonSM.SocialNetwork[j])!=0){
            printf("%s\n",array[i].PersonSM.SocialNetwork[j]);
        }
    }
    printf("Аккаунты в мессенджерах:\n");
    for (int j=0; j< MPS; j++){
        if (strlen(array[i].PersonSM.Messenger[j])!=0){
            printf("%s\n",array[i].PersonSM.Messenger[j]);
        }
    }      
    printf("\n");   
    return;
}


void deleteContact(Person array[], int* contactNum, int ID){
    int i = ID - 1;
    for(i;i<(*contactNum)-1;i++){
        array[i]=array[i+1];
        array[i].ID--;
    }
    memset(&array[(*contactNum)-1],0,sizeof(Person));
    (*contactNum)--;
    return;
}


void addContact(int* contactNum, Person array[]){ 
    if (*contactNum<MC){
        array[*contactNum].ID = *contactNum +1;
        clearCons();
        while (strlen(array[*contactNum].LastName) == 0){
            addField(array[*contactNum].LastName, "Введите Фамилию: ");
            if (strlen(array[*contactNum].LastName) == 0) {
            clearCons();
            printf("Ошибка! Фамилия не может быть пустой.\n");
            }       
        }
        clearCons();
        while (strlen(array[*contactNum].FirstName) == 0){
            addField(array[*contactNum].FirstName, "Введите имя: ");
            if (strlen(array[*contactNum].FirstName) == 0) {
            clearCons();
            printf("Ошибка! Имя не может быть пустым.\n");
            }       
        }
        clearCons();
        while (strlen(array[*contactNum].Patronymic) == 0){
            addField(array[*contactNum].Patronymic, "Введите отчество: ");
            if (strlen(array[*contactNum].Patronymic) == 0) {
            clearCons();
            printf("Ошибка! Отчество не может быть пустым.\n");
            }       
        }
        clearCons();
        printf("Хотите добавить место работы и должность? Y/N: ");

        switch (entEdit()){
            case 'y':
                clearCons();
                addField(array[*contactNum].PersonJob.Organization, "Введите место работы: ");
                clearCons();
                addField(array[*contactNum].PersonJob.Post, "Введите должность: ");
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
                addField(array[*contactNum].PhoneNumber[0], "Введите номер телефона: ");
                int i=1;
                while((choice == 'y') && i<5){
                    clearCons();
                    printf("Хотите добавить ещё один номер телефона (max %d)? Y/N: ", MPS);
                    choice = entEdit();
                    switch (choice){
                        case 'y':
                            clearCons();
                            addField(array[*contactNum].PhoneNumber[i], "Введите номер телефона: ");
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
                                if (strlen(array[*contactNum].PersonSM.Email[i])==0){
                                    ind = i;
                                    break;
                                }
                            }
                            if (ind != -1){
                                clearCons();
                                addField(array[*contactNum].PersonSM.Email[ind], "Введите электронную почту: ");
                            }
                            else{
                                clearCons();
                                printf("Места для почт не осталось\n");
                                break;
                            }
                            break;

                        case 's':
                            for (int i = 0; i<MPS; i++){
                                if (strlen(array[*contactNum].PersonSM.SocialNetwork[i])==0){
                                    ind = i;
                                    break;
                                }
                            }
                            if (ind != -1){
                                clearCons();
                                addField(array[*contactNum].PersonSM.SocialNetwork[ind], "Введите социальную сеть и свой логин: ");
                            }
                            else{
                                clearCons();
                                printf("Места социальных сетей не осталось\n");
                                break;
                            }
                            break;

                        case 'm':
                            for (int i = 0; i<MPS; i++){
                                if (strlen(array[*contactNum].PersonSM.Messenger[i])==0){
                                    ind = i;
                                    break;
                                }
                            }
                            if (ind != -1){
                                clearCons();
                                addField(array[*contactNum].PersonSM.Messenger[ind], "Введите мессенджер и свой логин: ");
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

        (*contactNum)++;
        return;
    }

    else{
        clearCons();
        printf("Слишком много контактов\n");
        return;
    } 
}


int main (){
    clearCons();
    Person contBook[MC];
    memset(contBook, 0, MC*sizeof(Person));
    int contactNum = 0;
    
    while (true){
        printf("A - добавить\nQ - выйти\nдля взаимедействия с контактом выберие его ID\n");
        for (int i=0; i<contactNum; i++){
                printf("%d) %s %s %s\n", contBook[i].ID, contBook[i].LastName,contBook[i].FirstName,contBook[i].Patronymic);
            }
        char inp[MF];
        fgets(inp, MF, stdin);
        clearCons();
        if (inp[0]>='0'&& inp[0]<='9'){
            int id = atoi(inp); 
            if (id <= contactNum) {
                showFullInfo(contBook, id); 
                printf("Меню действий над контактом:\nD - удалить\nC - изменить\nN - не изменять\n");
                switch(entEdit()){
                    case 'd':
                        deleteContact(contBook, &contactNum, id);
                        clearCons();
                        break;
                    case 'n':
                        clearCons();
                        break;
                    case 'c':
                        clearCons();
                        editContact(contBook, id);
                        clearCons();
                        break;
                }
            } else {
                printf("Неверный ID контакта!\n");
            }
        }
        else{
            inp[0]=tolower(inp[0]);
            switch (inp[0]){
                case 'a':
                    addContact(&contactNum, contBook);
                    break;
                case 'q':
                    return 0;
            }
        }
    }
}