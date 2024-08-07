#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define FILE_NAME "pipe_file.txt"
#define SEMAPHORE_NAME_FOR_PROC "/semaphore_for_proc"
#define SEMAPHORE_NAME_FOR_PARENT "/semaphore_for_parent"

int main(){
    size_t size;
    int read_from_file=0;
    sem_t* semid = sem_open(SEMAPHORE_NAME_FOR_PROC,O_RDWR);
    sem_t* sem_for_parrent = sem_open(SEMAPHORE_NAME_FOR_PARENT,O_RDWR);
    if (semid==SEM_FAILED){
        perror("semget error");
        exit(EXIT_FAILURE);
    }
    if (sem_for_parrent==SEM_FAILED){
        perror("semget error");
        exit(EXIT_FAILURE);
    }

    int fd = open(FILE_NAME,O_RDONLY|O_CREAT,0666);

    if (fd == -1) {
        perror("open file error");
        exit(EXIT_FAILURE);
    }
    int semstate=0;
    while(semstate==0){
        sem_getvalue(sem_for_parrent,&semstate);          //Проверка, происходит ли запись в файл.
    }
    sem_wait(semid); //Уменьшение места для чтения.
    while((size = read(fd, &read_from_file, sizeof(int))) > 0) {
        sleep(1);
        printf("procc read num %d\n",(read_from_file));
    }
    if(size==-1){
        perror("reading went wrong");
        close(fd);
        exit(EXIT_FAILURE);
    }
    sem_post(semid); //Увеличение места для чтения.
    printf("procc done with reading\n");
    close(fd);
    sem_close(semid);
    exit(EXIT_SUCCESS);
}