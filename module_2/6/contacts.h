#ifndef CONTACTS_H
#define CONTACTS_H

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


void clearCons();

int get_curn(char (*str)[MF]);

void addField(char str[MF], char text[]);

void addCase(char (*str)[MF]);

void editCase(char (*str)[MF]);

void deleteCase(char (*str)[MF]);

char entEdit();

void editContact(list *head, list *el);

list *showFullInfo(list *head, int id);

Person init_p();

void deleteContact(list **head, list *el);

void addContact(struct list **head, Person pers);


#endif 