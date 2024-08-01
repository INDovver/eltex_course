#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/ipc.h>
#include <unistd.h>

#define TERMINATE_MSG_TYPE 999
#define QUEUE_NAME "queueue"

int main() {
    mqd_t msgid = mq_open(QUEUE_NAME, 0666 | IPC_CREAT|O_CREAT);
    while (1) {
        scanf("%ld %100s",&message.msg_type,message.msg_text);
        while(getchar()!='\n');
        msgsnd(msgid, &message, sizeof(message.msg_text), 0);
        if(message.msg_type==TERMINATE_MSG_TYPE){
            printf("Exiting\n");
            exit(EXIT_SUCCESS);
        }
        msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0);
        if(message.msg_type==TERMINATE_MSG_TYPE){
            printf("Exiting\n");
            msgctl(msgid, IPC_RMID, NULL);
            exit(EXIT_SUCCESS);
        }
        printf("Received: %s\n", message.msg_text);
    }
}



