#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>  
#include <fcntl.h>
#include <semaphore.h>

#define WR 1
#define RD 0
#define FILE_NAME "pipe_file.txt"
#define SEMAPHORE_NAME "/semaphore"

void child_action(int *d, char *argv, int file_d, sem_t* semid){
    if(close(d[RD])==-1){
        perror("close for chiled failure");
        close(d[WR]);
        close(file_d);
        sem_unlink(SEMAPHORE_NAME);
        sem_close(semid);
        exit(EXIT_FAILURE);
    }

    int read_inf=0;
    ssize_t size;
    int random_num;

    for(int i =0; i < atoi(argv);i++){
        random_num = rand();
        if(write(d[WR],&random_num,sizeof(int))==-1){
            close(d[WR]);
            close(file_d);
            sem_unlink(SEMAPHORE_NAME);
            sem_close(semid);
            perror("writing goes wrong");
            exit(EXIT_FAILURE);
        }
        sleep(1);  
        sem_post(semid); //Ожидание записи в файл родительским процессом
        if(lseek(file_d,0,SEEK_SET)==-1){
            perror("lseeking goes wrong");
            close(d[WR]);
            close(file_d);
            sem_unlink(SEMAPHORE_NAME);
            sem_close(semid);
            exit(EXIT_FAILURE); 
        }
        while((size = read(file_d, &read_inf,sizeof(int)))>0){
            printf("num from file: %d\n",read_inf);
        }
        if (size==-1){
            perror("reading goes wrong");
            close(d[WR]);
            close(file_d);
            sem_unlink(SEMAPHORE_NAME);
            sem_close(semid);;
            exit(EXIT_FAILURE);
        }
    }
    sem_unlink(SEMAPHORE_NAME);
    sem_close(semid);
    close(d[WR]);
    close(file_d);
    exit(EXIT_SUCCESS);
}

void parent_action(int *d, int file_d,int pid, sem_t* semid){
    if(close(d[WR])==-1){
        perror("close for parent failure");
        close(d[RD]);
        close(file_d);
        sem_close(semid);
        exit(EXIT_FAILURE);
    }
    while(true){
        int read_num;
        ssize_t size;
        size = read(d[RD],&read_num,sizeof(int));
        if(size==-1){
            close(d[RD]);
            close(file_d);
            sem_close(semid);
            perror("reading goes wrong");
            exit(EXIT_FAILURE);
        }
        if(size==0){
            printf("reading is done\n");
            close(d[RD]);
            close(file_d);
            sem_close(semid);
            exit(EXIT_SUCCESS);
        }
        printf("parent proc read number is %d\n",read_num);
        if (write(file_d, &read_num, size)==-1){
            close(d[RD]);
            close(file_d);
            sem_close(semid);
            perror("write error");
            exit(EXIT_FAILURE);
        }
        sem_wait(semid);
    }
}

int main(int argc, char* argv[]){
    int pid;
    sem_t* sem;

    if (argc!=2){
        printf("too few or too much arguments\n");
        exit(EXIT_FAILURE);
    }
    int fd = open(FILE_NAME,O_RDWR|O_CREAT, 0666);
    if (fd == -1) {
        perror("file open error");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, 0) == -1) {
        perror("truncate error");
        close(fd);
        exit(EXIT_FAILURE);
    };
    
    if((sem = sem_open(SEMAPHORE_NAME,O_RDWR|O_CREAT,0666,0))==SEM_FAILED){
        perror("semget error");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe creation failed");
        sem_unlink(SEMAPHORE_NAME);
        sem_close(sem);
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    switch(pid = fork()){
        case -1:
            close(fd);
            sem_unlink(SEMAPHORE_NAME);
            sem_close(sem);
            close(pipe_fd[RD]);
            close(pipe_fd[WR]);
            perror("fork failure");
            exit(EXIT_FAILURE);
        case 0: child_action(pipe_fd, argv[1], fd, sem);
        default: parent_action(pipe_fd, fd, pid, sem);
    }
}