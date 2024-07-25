#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main (int argc, char *argv[]){
    size_t size=0;
    for(int i=1;i<argc;i++){
        size+=strlen(argv[i]);
    }
    size++;
    char *ptr = malloc(size);
    char *tmp = ptr;
    for(int i=1; i<argc;i++){
        strcpy(tmp,argv[i]);
        tmp+=strlen(argv[i]);
    }
    printf("%s\n",ptr);
    free(ptr);
    return 0;
}