#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>  
#include <fcntl.h>

#define WR 1
#define RD 0

void child_action(int *d, char *argv, int file_d){
    if(close(d[RD])==-1){
        perror("close for chiled failure");
        close(d[WR]);
        close(file_d);
        exit(EXIT_FAILURE);
    }
    for(int i =0; i < atoi(argv);i++){
        int random_num = rand();
        if(write(d[WR],&random_num,sizeof(int))==-1){
            perror("writing goes wrong");
            close(d[WR]);
            close(file_d);
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    close(d[WR]);
    close(file_d);
    exit(EXIT_SUCCESS);
}

void parent_action(int *d, int file_d){
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
            perror("reading goes wrong");
            close(d[RD]);
            close(file_d);
            exit(EXIT_FAILURE);
        }
        if(size==0){
            printf("reading is done\n");
            close(d[RD]);
            close(file_d);
            exit(EXIT_SUCCESS);
        }
        printf("read number is %d\n",read_num);
        if (write(file_d, &read_num, size)==-1){
            perror("write error");
            close(d[RD]);
            close(file_d);
            exit(EXIT_FAILURE);
        }
    };
}

int main(int argc, char* argv[]){
    if (argc!=2){
        printf("too few or too much arguments\n");
        exit(EXIT_FAILURE);
    }
    int fd = open("pipe_file.txt",O_RDWR|O_CREAT);
    if (fd == -1) {
        perror("Program error");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, 0) == -1) {
        perror("truncate error");
        close(fd);
        exit(EXIT_FAILURE);
    };
    
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
            perror("fork failure");
            close(fd);
            exit(EXIT_FAILURE);
        case 0: child_action(pipe_fd, argv[1], fd);
        default: parent_action(pipe_fd, fd);
    }
}