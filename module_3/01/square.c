#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>


void get_square(int argc, char *argv[], bool is_child){
    switch(is_child){
        case true: 
            for (int i = 1; i<argc; i+=2)
                printf("Child proc comutated square of %d square = %d\n", i, atoi(argv[i])*atoi(argv[i]));
            break;
        case false:
            for (int i = 2; i<argc; i+=2)
                printf("Parent roc comutated square of %d sqare = %d\n", i, atoi(argv[i])*atoi(argv[i]));
            break;
    }
}

int main (int argc, char *argv[]){
    pid_t pid;
    switch (pid = fork()){
        case -1: 
            perror("fork error");
            exit(EXIT_SUCCESS);
        case 0:
            get_square(argc, argv, true);
            exit(EXIT_SUCCESS);
        default:          
            get_square(argc, argv, false);
            exit(EXIT_SUCCESS);
    }
}