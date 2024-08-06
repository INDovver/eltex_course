#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>  
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define WR 1
#define RD 0
#define FILE_NAME "pipe_file.txt"
#define MAX_PROC_READ 2  //Определяет количетсово одновременно читающих процессов

void child_action(int *d, char *argv, int file_d, int semid){
    if(close(d[RD])==-1){
        perror("close for chiled failure");
        close(d[WR]);
        close(file_d);
        semctl(semid, 0, IPC_RMID);
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
            semctl(semid, 0, IPC_RMID);
            perror("writing goes wrong");
            exit(EXIT_FAILURE);
        }
        sleep(1);  
        semop(semid,(struct sembuf[2]){0,-1,0},2); //Ожидание записи в файл родительским процессом
        if(lseek(file_d,0,SEEK_SET)==-1){
            perror("lseeking goes wrong");
            close(d[WR]);
            close(file_d);
            semctl(semid, 0, IPC_RMID);
            exit(EXIT_FAILURE); 
        }
        while((size = read(file_d, &read_inf,sizeof(int)))>0){
            printf("num from file: %d\n",read_inf);
        }
        if (size==-1){
            perror("reading goes wrong");
            close(d[WR]);
            close(file_d);
            semctl(semid, 0, IPC_RMID);
            exit(EXIT_FAILURE);
        }
    }
    semctl(semid, 0, IPC_RMID);
    close(d[WR]);
    close(file_d);
    exit(EXIT_SUCCESS);
}

void parent_action(int *d, int file_d,int pid, int semid){
    if(close(d[WR])==-1){
        perror("close for parent failure");
        close(d[RD]);
        close(file_d);
        exit(EXIT_FAILURE);
    }
    while(true){
        int read_num;
        ssize_t size;
        size = read(d[RD],&read_num,sizeof(int));
        if(size==-1){
            close(d[RD]);
            close(file_d);
            perror("reading goes wrong");
            exit(EXIT_FAILURE);
        }
        if(size==0){
            printf("reading is done\n");
            close(d[RD]);
            close(file_d);
            exit(EXIT_SUCCESS);
        }
        semop(semid,&(struct sembuf){1,-MAX_PROC_READ,0},1); //Вычитание из семафора максимальное количество одновременных чтений из файла (если срабатывает, то никто не читает) 
        printf("parent proc read number is %d\n",read_num);
        if (write(file_d, &read_num, size)==-1){
            close(d[RD]);
            close(file_d);
            perror("write error");
            exit(EXIT_FAILURE);
        }
        semop(semid,(struct sembuf[2]){{0,1,0},{1,MAX_PROC_READ,0}},2); //Увеличение семафора чтения из файла на максимальное количество\
                                                                        одновременных чтений и допуск дочернего файла к след. итерации (генерация + чтение из файла).\
                                                                        Дочерний процесс работает несколько иначе, в отличии от остальных процессов, считывающих из файла\
                                                                        Так как ему нет смысла читать из файла, пока родитель на запишет туда что-то, т.к. они работают в связке.
    }
}

int main(int argc, char* argv[]){
    int pid;
    key_t key;
    int semid;

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
    
    if((key=ftok(FILE_NAME,64))==-1){
        perror("key error");
        exit(EXIT_FAILURE);
    }
    if((semid = semget(key,2,0666|IPC_CREAT))==-1){
        perror("semget error");
        exit(EXIT_FAILURE);
    }
    printf("semid is %d\n",semid);
    semctl(semid,1,SETVAL,MAX_PROC_READ);

    srand(time(NULL));
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe creation failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    switch(pid = fork()){
        case -1:
            close(fd);
            close(pipe_fd[RD]);
            close(pipe_fd[WR]);
            perror("fork failure");
            exit(EXIT_FAILURE);
        case 0: child_action(pipe_fd, argv[1], fd, semid);
        default: parent_action(pipe_fd, fd, pid, semid);
    }
}