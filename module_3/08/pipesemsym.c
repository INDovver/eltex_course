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
        semop(semid,&(struct sembuf){0,-1,0},1); //Уменьшнеие места в буфере генерации
        random_num = rand();
        if(write(d[WR],&random_num,sizeof(int))==-1){
            close(d[WR]);
            close(file_d);
            semctl(semid, 0, IPC_RMID);
            perror("writing goes wrong");
            exit(EXIT_FAILURE);
        }
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
        semop(semid,&(struct sembuf){1,1,0},1);  //Разрешение чтения для родительского процесса(Занесение записи в буфер канала)      
    }
    semop(semid,&(struct sembuf){0,-1,0},1); //Уменьшение места в буфере, но уже вне цикла
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

        semop(semid,&(struct sembuf){1,-1,0},1); //Уменьшение записей в буфере канала (запрет чтения)
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
        printf("parent proc read number is %d\n",read_num);
        if (write(file_d, &read_num, size)==-1){
            close(d[RD]);
            close(file_d);
            perror("write error");
            exit(EXIT_FAILURE);
        }
        semop(semid,&(struct sembuf){0,1,0},1); //Разрешение генерации (увеличение места в буфере генерации)
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
    int fd = open("pipe_file.txt",O_RDWR|O_CREAT, 0666);
    if (fd == -1) {
        perror("Program error");
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
    semctl(semid,0,SETVAL,1);

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