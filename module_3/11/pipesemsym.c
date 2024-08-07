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
#define SEMAPHORE_NAME_FOR_PROC "/semaphore_for_proc" //Семафор свободных мест для чтения (не распространяется на дочерний процесс).
#define SEMAPHORE_NAME_FOR_CHILD "/semaphore_for_child" //Семфор для дочернего процесса: когда родитеский заканчивает запись, он разрешает генерацию.
#define SEMAPHORE_NAME_FOR_PARENT "/semaphore_for_parent" //Т.к. нельзя уменьшить семофор больше чем на единицу, а вычесть максимальное кол-во процессов из семофора надо,\
                                                            то для атомарности вводится третий семофор(семофор для семофора).
#define MAX_PROC_READ 2  //Определяет количетсово одновременно читающих процессов

void child_action(int *d, char *argv, int file_d, sem_t* semid){
    if(close(d[RD])==-1){
        perror("close for chiled failure");
        close(d[WR]);
        close(file_d);
        sem_close(semid);
        sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
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
            sem_close(semid);
            sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
            perror("writing goes wrong");
            exit(EXIT_FAILURE);
        }
        sleep(1);  
        sem_wait(semid); //Ожидание записи в файл родительским процессом
        if(lseek(file_d,0,SEEK_SET)==-1){
            perror("lseeking goes wrong");
            close(d[WR]);
            close(file_d);
            sem_close(semid);
            sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
            exit(EXIT_FAILURE); 
        }
        while((size = read(file_d, &read_inf,sizeof(int)))>0){
            printf("num from file: %d\n",read_inf);
        }
        if (size==-1){
            perror("reading goes wrong");
            close(d[WR]);
            close(file_d);
            sem_close(semid);
            sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
            exit(EXIT_FAILURE);
        }
    }
    sem_close(semid);
    sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
    close(d[WR]);
    close(file_d);
    exit(EXIT_SUCCESS);
}

void parent_action(int *d, int file_d,int pid, sem_t *sems[]){
    if(close(d[WR])==-1){
        perror("close for parent failure");
        close(d[RD]);
        close(file_d);
        sem_close(sems[0]);
        sem_close(sems[1]);
        sem_close(sems[3]);
        sem_unlink(SEMAPHORE_NAME_FOR_PROC);
        exit(EXIT_FAILURE);
    }
    while(true){
        int read_num;
        ssize_t size;
        size = read(d[RD],&read_num,sizeof(int));
        if(size==-1){
            close(d[RD]);
            close(file_d);
            sem_close(sems[0]);
            sem_close(sems[1]);
            sem_close(sems[3]);
            sem_unlink(SEMAPHORE_NAME_FOR_PROC);
            perror("reading goes wrong");
            exit(EXIT_FAILURE);
        }
        if(size==0){
            printf("reading is done\n");
            close(d[RD]);
            close(file_d);
            sem_close(sems[0]);
            sem_close(sems[1]);
            sem_close(sems[3]);
            sem_unlink(SEMAPHORE_NAME_FOR_PROC);
            exit(EXIT_SUCCESS);
        }
        int semstate=0;
        while(semstate!=MAX_PROC_READ){
            sem_getvalue(sems[2],&semstate);    //Чтобы не начинал вычитать из семафора мест чтения, пока кто-то читает
        }
        sem_wait(sems[1]);                      //Запрет другим процессам влазить в процесс вычитания у родительского (атомарность вычитания)
        for(int i=MAX_PROC_READ;i>0;i--){
            sem_wait(sems[2]);                     //Вычитание из семафора максимальное количество одновременных чтений из файла (если срабатывает, то никто не читает) 
        }    
        sem_post(sems[1]);                         //Разрешение другим процессам начинать влазить, но теперь они будут блокироваться из-за отсутствия мест.
        printf("parent proc read number is %d\n",read_num);
        if (write(file_d, &read_num, size)==-1){
            close(d[RD]);
            close(file_d);
            sem_close(sems[0]);
            sem_close(sems[1]);
            sem_close(sems[3]);
            sem_unlink(SEMAPHORE_NAME_FOR_PROC);
            perror("write error");
            exit(EXIT_FAILURE);
        }
        for(int i=MAX_PROC_READ;i>0;i--){
            sem_post(sems[2]);                    //Освобождение места для чтения
        } 
        sem_post(sems[0]);                        //разрешение дочернему процессу генерировать
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    int pid;
    int fd;
    int pipe_fd[2];
    sem_t* sem_for_proc;
    sem_t* sem_for_child;
    sem_t* sem_for_parent;

    if (argc!=2){
        printf("too few or too much arguments\n");
        exit(EXIT_FAILURE);
    }
    if ((fd = open(FILE_NAME,O_RDWR|O_CREAT, 0666)) == -1) {
        perror("file open error");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, 0) == -1) {
        perror("truncate error");
        close(fd);
        exit(EXIT_FAILURE);
    };
    if((sem_for_child = sem_open(SEMAPHORE_NAME_FOR_CHILD,O_RDWR|O_CREAT,0666,0))==SEM_FAILED){
        perror("semget error");
        close(fd);
        exit(EXIT_FAILURE);
    }
    if((sem_for_proc = sem_open(SEMAPHORE_NAME_FOR_PROC,O_RDWR|O_CREAT,0666,MAX_PROC_READ))==SEM_FAILED){
        perror("semget error");
        close(fd);
        sem_close(sem_for_child);
        sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
        exit(EXIT_FAILURE);
    }
    if((sem_for_parent = sem_open(SEMAPHORE_NAME_FOR_PARENT,O_RDWR|O_CREAT,0666,1))==SEM_FAILED){
        perror("semget error");
        close(fd);
        sem_close(sem_for_child);
        sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
        close(fd);
        sem_close(sem_for_proc);
        sem_unlink(SEMAPHORE_NAME_FOR_PROC);
        exit(EXIT_FAILURE);
    }
    sem_t* sem_array[]={sem_for_child,sem_for_parent,sem_for_proc}; //для удобства передачи в родительский процесс.
    if (pipe(pipe_fd) == -1) {
        perror("pipe creation failed");
        close(fd);
        sem_close(sem_for_child);
        sem_close(sem_for_proc);
        sem_close(sem_for_parent);
        sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
        sem_unlink(SEMAPHORE_NAME_FOR_PARENT);
        sem_unlink(SEMAPHORE_NAME_FOR_PROC);
        exit(EXIT_FAILURE);
    }
    
    switch(pid = fork()){
        case -1:
            close(fd);
            close(pipe_fd[RD]);
            close(pipe_fd[WR]);
            perror("fork failure");
            sem_close(sem_for_child);
            sem_close(sem_for_proc);
            sem_close(sem_for_parent);
            sem_unlink(SEMAPHORE_NAME_FOR_CHILD);
            sem_unlink(SEMAPHORE_NAME_FOR_PROC);
            sem_unlink(SEMAPHORE_NAME_FOR_PARENT);
            exit(EXIT_FAILURE);
        case 0: child_action(pipe_fd, argv[1], fd, sem_for_child);
        default: parent_action(pipe_fd, fd, pid, sem_array);
    }
}