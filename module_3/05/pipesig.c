#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>  
#include <fcntl.h>
#include <signal.h>

#define WR 1
#define RD 0

volatile sig_atomic_t file_accessible = 1;

void accessibility(int signum) {
    switch(signum) {
        case SIGUSR1:
            file_accessible = 0;
            break;
        case SIGUSR2:
            file_accessible = 1;
            break;
    }
}

void child_action(int *d, char *argv, int file_d){
    if(close(d[RD])==-1){
        perror("close for chiled failure");
        close(d[WR]);
        close(file_d);
        exit(EXIT_FAILURE);
    }

    int read_inf=0;
    ssize_t size;
    int random_num;

    for(int i =0; i < atoi(argv);i++){
        if(lseek(file_d,0,SEEK_SET)==-1){
            perror("lseeking goes wrong");
            close(d[WR]);
            close(file_d);
            exit(EXIT_FAILURE); 
        }
        while((size = read(file_d, &read_inf,sizeof(int)))>0){
            printf("num from file: %d\n",read_inf);
        }
        if (size==-1){
            perror("reading goes wrong");
            close(d[WR]);
            close(file_d);
            exit(EXIT_FAILURE);
        }
        random_num = rand();
        if(write(d[WR],&random_num,sizeof(int))==-1){
            close(d[WR]);
            close(file_d);
            perror("writing goes wrong");
            exit(EXIT_FAILURE);
        }
        while(file_accessible==0)
            sleep(1);
    }
    if(lseek(file_d,0,SEEK_SET)==-1){
        perror("lseeking goes wrong");
        close(d[WR]);
        close(file_d);
        exit(EXIT_FAILURE); 
    }
    while((size = read(file_d, &read_inf,sizeof(int)))>0){
        printf("num from file: %d\n",read_inf);
    }
    close(d[WR]);
    close(file_d);
    exit(EXIT_SUCCESS);
}

void parent_action(int *d, int file_d,int pid){
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
        printf("parent proc read number is %d\n",read_num);
        if(kill(pid,SIGUSR1)==-1){
            perror("signal1 error");
            close(d[RD]);
            close(file_d);
            exit(EXIT_FAILURE);
        }
        if (write(file_d, &read_num, size)==-1){
            close(d[RD]);
            close(file_d);
            perror("write error");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        if(kill(pid,SIGUSR2)==-1){
            perror("signal2 error");
            close(d[RD]);
            close(file_d);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char* argv[]){
    if (argc!=2){
        printf("too few or too much arguments\n");
        exit(EXIT_FAILURE);
    }
    int fd = open("pipe_file.txt",O_RDWR|O_CREAT, 0644);
    if (fd == -1) {
        perror("Program error");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, 0) == -1) {
        perror("truncate error");
        close(fd);
        exit(EXIT_FAILURE);
    };
    
    signal(SIGUSR1, accessibility);
    signal(SIGUSR2, accessibility);
    srand(time(NULL));
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe creation failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    int pid;
    switch(pid = fork()){
        case -1:
            close(fd);
            close(pipe_fd[RD]);
            close(pipe_fd[WR]);
            perror("fork failure");
            exit(EXIT_FAILURE);
        case 0: child_action(pipe_fd, argv[1], fd);
        default: parent_action(pipe_fd, fd, pid);
    }
}