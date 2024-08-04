#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 100
#define TERMINATE_MSG_TYPE 999
#define FILE_NAME "ftok.txt"

typedef struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
}msg_buffer;

int main() {
    key_t key;
    int msgid;
    if((key = ftok(FILE_NAME, 65))==-1){
        perror("ftok error");
        exit(EXIT_FAILURE);
    }
    if ((msgid = msgget(key, 0666 | IPC_CREAT))==-1){
        perror("message queue error");
        exit(EXIT_FAILURE);
    }
    msg_buffer message;
    while (1) {
        printf("Send: ");
        scanf("%ld",&message.msg_type);
        while(getchar()!=' ');
        fgets(message.msg_text,sizeof(message.msg_text),stdin);
        message.msg_text[strcspn(message.msg_text,"\n")]='\0';
        if((msgsnd(msgid, &message, sizeof(message.msg_text), 0))==-1){
            perror("send error");
            exit(EXIT_FAILURE);
        }
        if(message.msg_type==TERMINATE_MSG_TYPE){
            printf("Exiting\n");
            msgctl(msgid, IPC_RMID, NULL);
            exit(EXIT_SUCCESS);
        }
        printf("Receive: ");
        fflush(stdout);
        if((msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0))==-1){
            perror("send error");
            exit(EXIT_FAILURE);
        }
        if(message.msg_type==TERMINATE_MSG_TYPE){
            printf("Exiting\n");
            exit(EXIT_SUCCESS);
        }
        printf("%s\n", message.msg_text);
    }
}



