#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SIZE sizeof(struct sockaddr_in) 

int client_sock;

void sign(int signum){
    printf("\n");
    close(client_sock);
    exit(EXIT_SUCCESS);
}

void sndfnct(int socket) {
    char buf[BUF_SIZE];
    while (1) {
        printf("You: ");
        fgets(buf, BUF_SIZE, stdin);
        buf[strcspn(buf,"\n")]='\0';
        send(socket, buf, BUF_SIZE, 0);
    }
}

void rcvnfnct(int socket) {
    char buf[BUF_SIZE];
    while (1) {
        recv(socket, buf, BUF_SIZE, 0);
        if(strncmp(buf,"exit",4)==0) {
            close(client_sock);
            break;
        }
        printf("\nReceived: %s\nYou: ", buf);
        fflush(stdout);
    }
}

int main(void) {
    signal(SIGUSR1,sign);
    int pid;
    struct sockaddr_in server_addr;

    if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, SIZE);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(connect(client_sock, (struct sockaddr *)&server_addr, SIZE)==-1){
        perror("Connect error");
        close(client_sock);
        exit(EXIT_FAILURE);
    }
    send(client_sock,NULL,0,0);

    switch (pid = fork()){
        case -1:
            perror("Fork error"); 
            exit(EXIT_FAILURE);
        case 0:
            sndfnct(client_sock);
        default:
            rcvnfnct(client_sock);
            kill(pid,SIGUSR1);
            wait(NULL);
    }
    exit(EXIT_SUCCESS);

    return 0;
}
