#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


#define MN 20
#define MF 2


typedef struct file_rights{
    char name[MN];
    mode_t rights;
} file_rights;


void chanmod(file_rights *file, int num, char (*str)[MN]){
    if (num == -1) {
        return;
    }
    if (strcspn(str[1], "+-=") == strlen(str[1])){
        file[num].rights=strtol(str[1], NULL, 8);
        return;
    }

    mode_t OM = 0;
    mode_t MM = 0;
    char action = '\0';

    for(int i =0; str[1][i]!='\0';i++){
        switch(str[1][i]){
            case 'u': OM|=0b111000000; break;
            case 'g': OM|=0b000111000; break;
            case 'o': OM|=0b000000111; break;
            case 'a': OM|=0b111111111; break;
            case '=':
            case '-':
            case '+': action = str[1][i]; break;
            case 'r': MM |= 0b100100100; break;
            case 'w': MM |= 0b010010010; break;
            case 'x': MM |= 0b001001001; break;
        }
        if (action!='\0' && (str[1][i+1] == '\0' || str[1][i+1]==',')){
            if (str[1][i+1]==',') i++;
            switch (action) {
                case '+': file[num].rights |= (OM & MM); break;
                case '-': file[num].rights &= ~(OM & MM); break;
                case '=': file[num].rights = (file[num].rights & ~OM) | (OM & MM); break;
            }
            OM = 0;
            MM = 0;
            action = '\0';
        }
    }
}


void print_mode(file_rights *file, int num){
    if (num == -1) return;
    mode_t mode = file[num].rights;
    printf("Права для %s:\n", file[num].name);
    printf("В цифровом виде: %.3o\n", mode);
    printf("В битовом виде: ");
    for (int i = 8; i >= 0; i--) {
        putchar((mode & (0b1 << i)) ? '1' : '0');
    }
    printf("\nВ буевенном виде: ");
    for (int i = 8; i >= 0; i--) {
        putchar((mode & (0b1 << i)) ? (i%3==2 ? 'r': (i%3==1 ? 'w': 'x')) : '-');
    }
    printf("\n");
}


void clear_buffer(){
    while(getchar()!='\n');
}


int valid_test(){
    char c = getchar();
    if (c=='\n'){
        printf("Ничего не введено!\n");
        return 1;
    }
    else{
        ungetc(c, stdin);
        return 0;
    }
}


int get_info(char* filename, int *count, file_rights* files){
    for (int i = 0; i < (*count); i++){
        if (strcmp(files[i].name, filename)==0){
            return i;
        }
    }
    struct stat buf;
    if (stat(filename, &buf)==0){
        strncpy(files[*count].name, filename, MN-1);
        files[*count].rights = buf.st_mode&0b111111111;
        (*count)++;
        return (*count)-1;
    }
    else printf("fail\n"); 
    return -1;    
}


int main(){
    int of_count = 0;
    file_rights files[MF];
    char string[3][MN];
    while (true){
        printf("Введите \"exit\" чтобы выйти;\n");
        if (!valid_test()){
            scanf("%s", string[0]);
            if (strcmp(string[0], "chmod")==0){
                scanf(" %s %s", string[1], string[2]);
                clear_buffer();
                chanmod(files, get_info(string[2], &of_count, files), string);
            }
            else if (strcmp(string[0], "exit")==0){
                break;
            }
            else{
                clear_buffer();
                print_mode(files, get_info(string[0], &of_count, files));
            }
        }
    }
}