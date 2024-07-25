#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define MA 25 // Max Args number
#define ML 100 // Max Length


int main(){
    char *arguments[MA];
    char input[ML];
    while(true){
        int i = 0;
        printf("Enter file name and args\n>> ");
        if (fgets(input,ML,stdin)==NULL||input[0]=='\n'){
            printf("Something goes wrong\n");
            continue;
        }
        input[strcspn(input,"\n")]=0;
        arguments[i]=strtok(input," ");    
        if (strcmp(arguments[i],"exit")==0)
            return 0;                          
        while(arguments[i]!=NULL&&i<MA){
            i++;
            arguments[i]=strtok(NULL," ");
        }
        arguments[i]=NULL; 
        pid_t pid;
        switch(pid=fork()){
            case -1:
                perror("Fork error");
                continue;
            case 0:
                execv(arguments[0], arguments);
                perror("Exec error");
                exit(EXIT_FAILURE);
            default:
                waitpid(pid, NULL, 0);
        }
    }
}