#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#define FILE_NAME "pipe_file.txt"

int main(){
    size_t size;
    int read_from_file=0;
    key_t key = ftok(FILE_NAME,64);
    pid_t selfpid = getpid();
    int semid = semget(key,2,0);
    printf("semid is %d\n",semid);
    if (semid==-1){
        perror("semget error");
        exit(EXIT_FAILURE);
    }

    int fd = open(FILE_NAME,O_RDWR|O_CREAT,0666);

    if (fd == -1) {
        perror("open file error");
        exit(EXIT_FAILURE);
    }
    semop(semid,&(struct sembuf){1,-1,1},1); //Уменьшение места для чтения
    while((size = read(fd, &read_from_file, sizeof(int))) > 0) {
        sleep(1);
        printf("procc %d read num %d\n",(int)selfpid,read_from_file);
    }
    if(size==-1){
        perror("reading went wrong");
        close(fd);
        exit(EXIT_FAILURE);
    }
    semop(semid,&(struct sembuf){1,1,1},1); //Увеличение места для чтения
    printf("procc %d done with reading\n",(int)selfpid);
    close(fd);
    exit(EXIT_SUCCESS);
}