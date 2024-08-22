#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SIZE sizeof(struct sockaddr_in) // для упрощения

int main() {
    int server_sock, pid;
    struct sockaddr_in server_addr, client_addr1, client_addr2;
    char buf[BUF_SIZE];
    socklen_t addr_len = SIZE;

    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, SIZE);
    memset(&client_addr1, 0, SIZE);
    memset(&client_addr2, 0, SIZE);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (const struct sockaddr *)&server_addr, SIZE) < 0) {
        perror("Bind error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Start\n");

    recvfrom(server_sock,NULL,0,0,(struct sockaddr*)&client_addr1,&addr_len);
    client_addr2=client_addr1;
    while(memcmp(&client_addr2, &client_addr1, SIZE) == 0)
        recvfrom(server_sock,NULL,0,0,(struct sockaddr*)&client_addr2,&addr_len);

    printf("Both users connected\n");

    while(strncmp(buf,"exit",4)!=0){
        recvfrom(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&server_addr,&addr_len);
        if(memcmp(&server_addr, &client_addr1, SIZE) == 0)
            sendto(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&client_addr2,SIZE);
        else
            sendto(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&client_addr1,SIZE);
    }
    sendto(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&client_addr1,SIZE);
    sendto(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&client_addr2,SIZE);

    close(server_sock);
    return 0;
}
